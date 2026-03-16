// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "HW9_GameStateBase.generated.h"


/**
 * 
 */
UCLASS()
class NBC_CH4_HW9_API AHW9_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHW9_GameStateBase();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void ResetGame();
	
	void StartTurnTimer(const FString& InTurnPlayerName, float InTurnToralTime);
	
	void SetAnswerString(const FString& InAnswerString);
	FORCEINLINE const FString& GetAnswerString() const { return AnswerString; }
	bool IsTimeOut() const;;
	FORCEINLINE int32 GetMaxGuessCount() const { return MaxGuessCount; }

protected:
	UFUNCTION()
	void OnRep_TurnRemainingTime();
	
private:
	UFUNCTION()
	void UpdateTurnTimerUI();
protected:
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxGuessCount;

	UPROPERTY(EditDefaultsOnly)
	double TurnTimerRate;
	
	UPROPERTY(Replicated)
	FString TurnPlayerName;
	
	UPROPERTY(ReplicatedUsing = OnRep_TurnRemainingTime)
	double TurnRemainTime;	
	
	UPROPERTY(Replicated)
	double TurnTotalTime;


	
private:
	FString AnswerString;
	
	FTimerHandle TurnTimerHandle;
	double LastTurnTimerUpdateTime;
	
	TWeakObjectPtr<UTimerSubsystem> TimerSubsystem;
	
	
};
