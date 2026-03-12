// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HW9_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NBC_CH4_HW9_API AHW9_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AHW9_PlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Init(const FString& InPlayerName, int32 InMaxGuessCount);
	bool TryIncreaseGuessCount();
	FORCEINLINE bool HasRemainGuess() const { return CurrentGuessCount < MaxGuessCount; }
	void ResetCount(int32 InMaxGuessCount);
	void Clear();
	
	FString GetPlayerInfo() const;
	FORCEINLINE FString GetHW9PlayerName() const{return PlayerName;};
private:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
