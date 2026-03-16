// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HW9_GameStateBase.h"

#include "Game/HW9_GameModeBase.h"
#include "Game/TimerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AHW9_GameStateBase::AHW9_GameStateBase()
	: MaxGuessCount(3),
	  TurnTimerRate(0.01f),
	  TurnRemainTime(0),
	  TurnTotalTime(0)
{
	SetNetUpdateFrequency(1);
}

void AHW9_GameStateBase::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		TimerSubsystem = World->GetSubsystem<UTimerSubsystem>();
	}
}

void AHW9_GameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (UWorld* World = GetWorld())
	{
		FTimerManager& WorldTimerManager = World->GetTimerManager();
		WorldTimerManager.ClearTimer(TurnTimerHandle);
	}
}

void AHW9_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TurnRemainTime);
	DOREPLIFETIME(ThisClass, TurnPlayerName);
	DOREPLIFETIME(ThisClass, TurnTotalTime);
}

void AHW9_GameStateBase::ResetGame()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	TurnTotalTime = 0.0;
	TurnRemainTime = 0.0;
	LastTurnTimerUpdateTime = 0.0;
	AnswerString = TEXT("");
	TurnPlayerName = TEXT("");
}

void AHW9_GameStateBase::SetAnswerString(const FString& InAnswerString)
{
	if (HasAuthority())
	{
		AnswerString = InAnswerString;
	}
}

bool AHW9_GameStateBase::IsTimeOut() const
{
	return TurnRemainTime <= 0.0;
}

void AHW9_GameStateBase::StartTurnTimer(const FString& InTurnPlayerName, float InTurnToralTime)
{
	if (HasAuthority() == false)
	{
		return;
	}
	TurnPlayerName = InTurnPlayerName;
	TurnTotalTime = InTurnToralTime;
	TurnRemainTime = InTurnToralTime;
	ForceNetUpdate();
	
	LastTurnTimerUpdateTime = GetWorld()->GetTimeSeconds();
	FTimerManager& WorldTimerManager = GetWorldTimerManager();
	WorldTimerManager.ClearTimer(TurnTimerHandle);
	WorldTimerManager.SetTimer(TurnTimerHandle, this, &ThisClass::UpdateTurnTimerUI, TurnTimerRate, true);
}

void AHW9_GameStateBase::OnRep_TurnRemainingTime()
{
	if (TurnRemainTime > 0.0)
	{
		if (TimerSubsystem.IsValid())
		{
			TimerSubsystem->OnTurnRemainTimeChanged.Broadcast(TurnPlayerName, TurnRemainTime, TurnTotalTime);
		}
		FTimerManager& WorldTimerManager = GetWorldTimerManager();

		if (WorldTimerManager.TimerExists(TurnTimerHandle) == false)
		{
			LastTurnTimerUpdateTime = GetWorld()->GetTimeSeconds();
			WorldTimerManager.SetTimer(TurnTimerHandle, this, &ThisClass::UpdateTurnTimerUI, TurnTimerRate, true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	}
}


void AHW9_GameStateBase::UpdateTurnTimerUI()
{
	double CurrentTime = GetWorld()->GetTimeSeconds();
	double DeltaTime = CurrentTime - LastTurnTimerUpdateTime;
	LastTurnTimerUpdateTime = CurrentTime;
	TurnRemainTime -= DeltaTime;
	if (TurnRemainTime < 0)
	{
		TurnRemainTime = 0.0f;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TurnTimerHandle);
		}
		if (TimerSubsystem.IsValid())
		{
			TimerSubsystem->OnTurnRemainTimeChanged.Broadcast(TurnPlayerName, TurnRemainTime, TurnTotalTime);
		}

		if (HasAuthority())
		{
			if (UWorld* World = GetWorld())
			{
				AHW9_GameModeBase* GameModeBase = World->GetAuthGameMode<AHW9_GameModeBase>();
				if (IsValid(GameModeBase))
				{
					GameModeBase->EndTurn(true);
				}
			}
		}
	}else
	{
		if (TimerSubsystem.IsValid())
		{
			TimerSubsystem->OnTurnRemainTimeChanged.Broadcast(TurnPlayerName, TurnRemainTime, TurnTotalTime);
		}
	}
}
