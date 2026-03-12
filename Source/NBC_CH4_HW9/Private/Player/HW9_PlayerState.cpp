// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW9_PlayerState.h"

#include "Game/HW9_GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AHW9_PlayerState::AHW9_PlayerState()
{
	SetReplicates(true);
}

void AHW9_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

void AHW9_PlayerState::Init(const FString& InPlayerName, int32 InMaxGuessCount)
{
	PlayerName = InPlayerName;
	CurrentGuessCount = 0;
	MaxGuessCount = InMaxGuessCount;
}

bool AHW9_PlayerState::TryIncreaseGuessCount()
{
	if (CurrentGuessCount >= MaxGuessCount)
	{
		CurrentGuessCount = MaxGuessCount;
		return false;
	}
	++CurrentGuessCount;
	return true;
}

void AHW9_PlayerState::ResetCount(int32 InMaxGuessCount)
{
	CurrentGuessCount = 0;
	MaxGuessCount = InMaxGuessCount;
}

void AHW9_PlayerState::Clear()
{
	PlayerName = "";
	CurrentGuessCount = 0;
}

FString AHW9_PlayerState::GetPlayerInfo() const
{
	FString Ret = FString::Printf(TEXT("%s (%d/%d)"), *PlayerName, CurrentGuessCount,MaxGuessCount);
	return Ret;
}
