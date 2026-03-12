// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "HW9_HUD.generated.h"

class UEditableText;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NBC_CH4_HW9_API UHW9_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnChatInputTextCommitted( const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void AddChatOutputText(const FString& ChatString);
	
	UFUNCTION()
	void SetNotificationText(const FString& NotificationString);
	UFUNCTION()
	void ClearNotificationText();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotificationText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> ChatInputBox;
	
	UPROPERTY(meta = (BindWidget,MultiLine=true))
	TObjectPtr<UTextBlock> ChatOutputBox;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Chat")
	int32 MaxOutputRowNum;
private:
	int32 OutputRowNum;
	
	FTimerHandle NotificationTimerHandle;
};
