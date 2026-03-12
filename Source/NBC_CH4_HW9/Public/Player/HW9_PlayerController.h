// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW9_PlayerController.generated.h"

class UHW9_HUD;
/**
 * 
 */
UCLASS()
class NBC_CH4_HW9_API AHW9_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void CommitChat(const FString& ChatString);

	UFUNCTION(Client, Reliable)
	void ClientRPCNotify(const FString& NotificationString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCCommitChat(const FString& ChatString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCCommitChat(const FString& ChatString);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UHW9_HUD> HUDClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UHW9_HUD> HUDInstance;
};
