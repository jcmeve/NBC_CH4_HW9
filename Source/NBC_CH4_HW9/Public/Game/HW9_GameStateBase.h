// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void SetAnswerString(const FString& InAnswerString);
	FORCEINLINE const FString& GetAnswerString() const { return AnswerString; };
	FORCEINLINE int32 GetMaxGuessCount() const { return MaxGuessCount; }

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 MaxGuessCount;
private:
	FString AnswerString;
};
