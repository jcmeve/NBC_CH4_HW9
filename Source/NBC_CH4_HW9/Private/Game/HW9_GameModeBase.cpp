// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HW9_GameModeBase.h"

#include "Game/HW9_GameStateBase.h"
#include "Player/HW9_PlayerController.h"
#include "Player/HW9_PlayerState.h"
#include "Runtime/Core/Tests/Containers/TestUtils.h"

AHW9_GameModeBase::AHW9_GameModeBase()
	: BaseballStringLength(3),
	  TurnTime(10.0)
{
}

void AHW9_GameModeBase::BroadcastChat(FString ChatResultMessage)
{
	for (AHW9_PlayerController* PlayerController : PlayerControllers)
	{
		if (IsValid(PlayerController))
		{
			PlayerController->ClientRPCCommitChat(ChatResultMessage);
		}
	}
}

void AHW9_GameModeBase::BroadcastNotification(FString NotificationString)
{
	for (AHW9_PlayerController* PlayerController : PlayerControllers)
	{
		if (IsValid(PlayerController))
		{
			PlayerController->ClientRPCNotify(NotificationString);
		}
	}
}

void AHW9_GameModeBase::ChatCommit(const AHW9_PlayerController* InChattingPlayerController,
                                   const FString& InChatMessage)
{
	bool bIsGameEnd = false;
	if (IsValid(InChattingPlayerController) == false)
	{
		return;
	}
	AHW9_PlayerState* ChattingPlayerState = InChattingPlayerController->GetPlayerState<AHW9_PlayerState>();
	if (IsValid(ChattingPlayerState) == false)
	{
		return;
	}

	bool bIsTimeOut = false;
	AHW9_GameStateBase* GameStateBase = GetGameState<AHW9_GameStateBase>();
	if (IsValid(GameStateBase))
	{
		bIsTimeOut = GameStateBase->IsTimeOut();
	}
	check(PlayerControllers.IsValidIndex(CurrentPlayerIndex));
	if (bIsTimeOut == false && PlayerControllers[CurrentPlayerIndex] == InChattingPlayerController &&  IsGuessNumberString(InChatMessage))
	{
		
		FString ChatResultMessage(TEXT("[야구게임]"));
		if (ChattingPlayerState->TryIncreaseGuessCount() == false)
		{
			ChatResultMessage.Append(TEXT(" ") + ChattingPlayerState->GetPlayerInfo());
			ChatResultMessage.Append(TEXT(": 도전 기회를 이미 소진하였습니다."));
			ChatResultMessage.Append(InChatMessage);
		}
		else
		{
			FBaseballResult Result = CheckAnswer(InChatMessage);
			if (Result.Strike == BaseballStringLength)
			{
				bIsGameEnd = true;
				FString NotificationString = ChattingPlayerState->GetHW9PlayerName() + TEXT(" Win!");
				BroadcastNotification(NotificationString);
			}
			else
			{
				if (CheckDraw())
				{
					bIsGameEnd = true;
					BroadcastNotification(TEXT(" Draw!"));
				}
			}
			ChatResultMessage.Append(TEXT(" ") + ChattingPlayerState->GetPlayerInfo());
			ChatResultMessage.Append(InChatMessage);
			FString BaseballResultString(TEXT(""));
			if (Result.Strike == 0 && Result.Ball == 0)
			{
				BaseballResultString = FString::Printf(TEXT("[OUT]"));
			}
			else
			{
				BaseballResultString = FString::Printf(TEXT("[%dS %dB]"), Result.Strike, Result.Ball);
			}
			ChatResultMessage.Append(BaseballResultString);
		}
		BroadcastChat(ChatResultMessage);
		if (!bIsGameEnd)
		{
			EndTurn(false);
		}
	}
	else
	{
		FString ChatResultMessage(TEXT("[일반채팅]"));
		ChatResultMessage.Append(TEXT(" ") + ChattingPlayerState->GetPlayerInfo());
		ChatResultMessage.Append(TEXT(" ") + InChatMessage);

		BroadcastChat(ChatResultMessage);
	}
	if (bIsGameEnd)
	{
		ResetGame();
	}
}

void AHW9_GameModeBase::EndTurn(bool bTimeOut)
{
	if (bTimeOut)
	{
		check(PlayerControllers.IsValidIndex(CurrentPlayerIndex));
		if (AHW9_PlayerState* PlayerState = PlayerControllers[CurrentPlayerIndex]->GetPlayerState<AHW9_PlayerState>())
		{
			PlayerState->TryIncreaseGuessCount();
		}
	}
	NextTurn();
}

void AHW9_GameModeBase::NextTurn()
{
	if (PlayerControllers.IsEmpty())
	{
		return;
	}
	CurrentPlayerIndex = (CurrentPlayerIndex + 1) % InGamePlayerNum;
	AHW9_GameStateBase* GameStateBase = GetGameState<AHW9_GameStateBase>();
	if (IsValid(GameStateBase))
	{
		AHW9_PlayerState* PlayerState = PlayerControllers[CurrentPlayerIndex]->GetPlayerState<AHW9_PlayerState>();
		if (PlayerState)
		{
			GameStateBase->StartTurnTimer(PlayerState->GetHW9PlayerName(), TurnTime);
		}
	}
}

void AHW9_GameModeBase::SkipTurn()
{
	NextTurn();
}



bool AHW9_GameModeBase::CheckDraw()
{
	for (AHW9_PlayerController* PlayerController : PlayerControllers)
	{
		if (IsValid(PlayerController))
		{
			AHW9_PlayerState* PlayerState = PlayerController->GetPlayerState<AHW9_PlayerState>();
			if (IsValid(PlayerState))
			{
				if (PlayerState->HasRemainGuess())
				{
					return false;
				}
			}
		}
	}
	return true;
}

void AHW9_GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	AHW9_PlayerController* PlayerController = Cast<AHW9_PlayerController>(NewPlayer);
	if (IsValid(PlayerController))
	{
		PlayerControllers.Add(PlayerController);
		AHW9_PlayerState* PlayerState = PlayerController->GetPlayerState<AHW9_PlayerState>();
		if (IsValid(PlayerState))
		{
			FString PlayerStateName = FString::Printf(TEXT("Player%d"), PlayerControllers.Num());
			PlayerState->Init(PlayerStateName, 0);
		}
	}
	if (PlayerControllers.Num() <= 2 || CheckDraw())
	{
		ResetGame();
	}
}

void AHW9_GameModeBase::Logout(AController* Exiting)
{
	AHW9_PlayerController* PlayerController = Cast<AHW9_PlayerController>(Exiting);
	bool bShouldSkipTurn = false;
	if (PlayerControllers.IsValidIndex(CurrentPlayerIndex) && PlayerControllers[CurrentPlayerIndex] == PlayerController)
	{
		bShouldSkipTurn = true;
	}
	if (IsValid(PlayerController))
	{
		PlayerControllers.RemoveSingle(PlayerController);
	}
	if (PlayerControllers.Num() < 2 || CheckDraw())
	{
		ResetGame();
	}
	else
	{
		InGamePlayerNum--;
	}

	if (bShouldSkipTurn)
	{
		CurrentPlayerIndex--;
		SkipTurn();
	}

	Super::Logout(Exiting);
}

FBaseballResult AHW9_GameModeBase::CheckAnswer(const FString& GuessString) const
{
	check(GuessString.Len() == BaseballStringLength);
	FBaseballResult Result;

	AHW9_GameStateBase* GameStateBase = GetGameState<AHW9_GameStateBase>();
	if (IsValid(GameStateBase) == false)
	{
		return Result;
	}
	const FString& AnswerString = GameStateBase->GetAnswerString();
	for (int32 i = 0; i < BaseballStringLength; i++)
	{
		int32 Index = -1;
		if (AnswerString[i] == GuessString[i])
		{
			++Result.Strike;
		}
		else if (AnswerString.FindChar(GuessString[i], Index))
		{
			++Result.Ball;
		}
	}
	return Result;
}

void AHW9_GameModeBase::ResetGame()
{
	AHW9_GameStateBase* GameStateBase = GetGameState<AHW9_GameStateBase>();
	if (IsValid(GameStateBase) == false)
	{
		return;
	}
	CurrentPlayerIndex = -1;
	InGamePlayerNum = PlayerControllers.Num();
	GameStateBase->ResetGame();
	GameStateBase->SetAnswerString(GenerateRandomNumbers());
	for (AHW9_PlayerController* PlayerController : PlayerControllers)
	{
		if (IsValid(PlayerController))
		{
			AHW9_PlayerState* PlayerState = PlayerController->GetPlayerState<AHW9_PlayerState>();
			if (IsValid(PlayerState))
			{
				PlayerState->ResetCount(GameStateBase->GetMaxGuessCount());
			}
			PlayerController->ClientRPCCommitChat(TEXT("게임을 시작합니다"));
		}
	}
	NextTurn();
}


bool AHW9_GameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	if (InNumberString.Len() != BaseballStringLength)
	{
		return false;
	}
	TSet<int32> Set;
	for (int32 i = 0; i < BaseballStringLength; i++)
	{
		if (FChar::IsDigit(InNumberString[i]) == false)
		{
			return false;
		}
		if (Set.Contains(InNumberString[i]))
		{
			return false;
		}
		Set.Add(InNumberString[i]);
	}
	return true;
}

FString AHW9_GameModeBase::GenerateRandomNumbers()
{
	FString AnswerString = TEXT("");
	FMath::RandInit(FDateTime::Now().GetTicks());
	TArray<int32> Numbers;
	Numbers.SetNum(9);
	for (int32 i = 1; i < 9; ++i)
	{
		Numbers[i - 1] = i;
	}

	for (int32 i = 0; i < BaseballStringLength; i++)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		AnswerString.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAtSwap(Index);
	}
	UE_LOG(LogGameMode, Log, TEXT("Generated random number : %s"), *AnswerString);
	return AnswerString;
}
