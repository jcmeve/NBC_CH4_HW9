// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HW9_HUD.h"

#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Player/HW9_PlayerController.h"

void UHW9_HUD::NativeConstruct()
{
	Super::NativeConstruct();
	if (ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	OutputRowNum = 0;
}

void UHW9_HUD::NativeDestruct()
{
	Super::NativeDestruct();
	if (ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		ChatInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	GetWorld()->GetTimerManager().ClearTimer(NotificationTimerHandle);
}

void UHW9_HUD::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		AHW9_PlayerController* PlayerController = Cast<AHW9_PlayerController>(GetOwningPlayer());
		if (IsValid(PlayerController))
		{
			PlayerController->CommitChat(Text.ToString());
			ChatInputBox->SetText(FText());
		}
	}
}

void UHW9_HUD::AddChatOutputText(const FString& ChatString)
{
	++OutputRowNum;
	FString OutputChatLogs = ChatOutputBox->GetText().ToString();
	if (OutputRowNum > MaxOutputRowNum)
	{
		OutputRowNum = MaxOutputRowNum;
		int32 Index;
		OutputChatLogs.FindChar(TEXT('\n'), Index);
		OutputChatLogs = OutputChatLogs.Right(OutputChatLogs.Len() - Index);
	}
	OutputChatLogs.Append(ChatString + TEXT('\n'));
	ChatOutputBox->SetText(FText::FromString(OutputChatLogs));
}

void UHW9_HUD::SetNotificationText(const FString& NotificationString)
{
	NotificationText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(NotificationTimerHandle, this, &UHW9_HUD::ClearNotificationText, 5.0f, false);
	NotificationText->SetText(FText::FromString(NotificationString));
}

void UHW9_HUD::ClearNotificationText()
{
	NotificationText->SetVisibility(ESlateVisibility::Hidden);
}
