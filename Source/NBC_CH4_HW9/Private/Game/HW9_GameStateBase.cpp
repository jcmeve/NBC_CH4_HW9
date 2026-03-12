// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HW9_GameStateBase.h"

AHW9_GameStateBase::AHW9_GameStateBase()
	:MaxGuessCount(3)
{
}

void AHW9_GameStateBase::SetAnswerString(const FString& InAnswerString)
{
	if (HasAuthority()){
		AnswerString = InAnswerString;
	}
}
