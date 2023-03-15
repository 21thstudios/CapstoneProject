// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameInstance.h"

USessionGameInstance::USessionGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnStartOnlineGameComplete);
}
