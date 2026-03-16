// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimerSubsystem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTurnRemainTimeChanged, 
	const FString&, TurnPlayerName, 
	double, TurnRemainingTime, 
	double, TurnTotalTime);

/**
 * 
 */
UCLASS()
class NBC_CH4_HW9_API UTimerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnTurnRemainTimeChanged OnTurnRemainTimeChanged;
};
