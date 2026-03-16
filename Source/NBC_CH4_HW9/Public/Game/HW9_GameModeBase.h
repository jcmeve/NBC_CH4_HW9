// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HW9_GameModeBase.generated.h"

class AHW9_PlayerController;
/**
 * 
 */
struct FBaseballResult
{
	FBaseballResult():Strike(0),Ball(0){}
	int32 Strike;
	int32 Ball;
};


UCLASS()
class NBC_CH4_HW9_API AHW9_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHW9_GameModeBase();

	void ChatCommit(const AHW9_PlayerController* InChattingPlayerController, const FString& InChatMessage);
	void EndTurn(bool bTimeOut);
	void NextTurn();
	void SkipTurn();

protected:
	bool CheckDraw();
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Baseball")
	int32 BaseballStringLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Baseball")
	double TurnTime;
private:
	void BroadcastChat(FString ChatResultMessage);
	void BroadcastNotification(FString NotificationString);
	
	FBaseballResult CheckAnswer(const FString& GuessString) const;
	void ResetGame();
	bool IsGuessNumberString(const FString& InNumberString);
	FString GenerateRandomNumbers();
	UPROPERTY()
	TArray<TObjectPtr<AHW9_PlayerController>> PlayerControllers;
	
private:
	int32 CurrentPlayerIndex;
	int32 InGamePlayerNum;
};
