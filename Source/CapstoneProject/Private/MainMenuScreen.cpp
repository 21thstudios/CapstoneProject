// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuScreen.h"


void UMainMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnCreateSessionComplete);
	OnCreateSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnStartOnlineGameComplete);
}

void UMainMenuScreen::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UMainMenuScreen::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN,
                                  bool bIsPresence, int32 MaxNumPlayers)
{
	
}

void UMainMenuScreen::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMainMenuScreen::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
}
