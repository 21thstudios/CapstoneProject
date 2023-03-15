// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

const FString HOST_MAP_DESTINATION_NAME = TEXT("FirstPersonMap");

USessionGameInstance::USessionGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnStartOnlineGameComplete);
}

bool USessionGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,
	bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	if (IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 5;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
			SessionSettings->Set(SETTING_MAPNAME, HOST_MAP_DESTINATION_NAME, EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			
			UE_LOG(LogTemp, Display, TEXT("Session Interface is valid. Attempting to create session..."));
			return Session->CreateSession(*UserId, SessionName, *SessionSettings);
		}
		UE_LOG(LogTemp, Error, TEXT("Unable to initiate session creation due to invalid Session Interface or UserId"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to initiate session creation due to uninitialized Online Subsystem!"));
	}
	return false;
}

void USessionGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("Session creation with SessionName: %s has completed. Session creation %hs"), *SessionName.ToString(),  bWasSuccessful ? "succeeded" : "failed");
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid()) 
		{
			Session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				
				// OnStartSessionComplete delegate triggered upon completion
				Session->StartSession(SessionName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Session creation completed, but the SessionInterface is no longer valid and therefore the session cannot be started."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session creation completed, but the OnlineSubsystem is no longer initialized and therefore the session cannot be started."));
	}
}