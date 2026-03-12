// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW9_PlayerController.h"

#include "Game/HW9_GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HW9_HUD.h"

void AHW9_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() == false)
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	SetShowMouseCursor(true);
	
	if (IsValid(HUDClass))
	{
		HUDInstance = CreateWidget<UHW9_HUD>(this, HUDClass);
		HUDInstance->AddToViewport();
	}
}

void AHW9_PlayerController::CommitChat(const FString& ChatString)
{
	if (IsLocalController())
	{
		ServerRPCCommitChat(ChatString);
	}
}

void AHW9_PlayerController::ClientRPCNotify_Implementation(const FString& NotificationString)
{
	if (IsValid(HUDInstance))
	{
		HUDInstance->SetNotificationText(NotificationString);
	}
}

void AHW9_PlayerController::ClientRPCCommitChat_Implementation(const FString& ChatString)
{
	if (IsValid(HUDInstance)){
		HUDInstance->AddChatOutputText(ChatString);
	}
}

void AHW9_PlayerController::ServerRPCCommitChat_Implementation(const FString& ChatString)
{
	AHW9_GameModeBase* GameModeBase = Cast<AHW9_GameModeBase> (UGameplayStatics::GetGameMode(this));
	if (IsValid(GameModeBase))
	{
		GameModeBase->ChatCommit(this, ChatString);
	}
}
