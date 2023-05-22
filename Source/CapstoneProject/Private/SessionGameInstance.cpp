#include "SessionGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

const FName SESSION_NAME = FName(TEXT("TestSessionName"));
const FString MAIN_MENU_MAP_NAME = TEXT("MainMenu");
const FString HOST_MAP_DESTINATION_NAME = TEXT("/Game/Maps/BloodGulch/BloodGulch");

USessionList* MenuWidgetHandle;

USessionGameInstance::USessionGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	HostedSessionInfo.ServerName = SESSION_NAME;
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnDestroySessionComplete);
}

bool USessionGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,
                                       bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, bool bUseLobbies)
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 20;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			if (bUseLobbies)
			{
				SessionSettings->bUseLobbiesIfAvailable = bUseLobbies;
			}
			//SessionSettings->bAllowJoinViaPresence = bIsPresence;
			//SessionSettings->bAllowJoinViaPresenceFriendsOnly = bIsPresence;
			SessionSettings->bIsDedicated= false;

			//SessionSettings->Set(SETTING_MAPNAME, HOST_MAP_DESTINATION_NAME, EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			
			UE_LOG(LogTemp, Display, TEXT("Session Interface is valid. Attempting to create session..."));
			return Session->CreateSession(0, SESSION_NAME, *SessionSettings);
		}
		UE_LOG(LogTemp, Error, TEXT("Unable to initiate session creation due to invalid Session Interface or UserId!"));
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
				Session->StartSession(SESSION_NAME);
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

void USessionGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("Starting Online Game with SessionName: %s has completed. Online game creation %hs"), *SessionName.ToString(),  bWasSuccessful ? "succeeded" : "failed");
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
		if (bWasSuccessful)
		{
			const ULocalPlayer* Player = GetFirstGamePlayer();
			const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
			HostedSessionInfo.ServerName = SessionName;
			IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
			IOnlineIdentity* OnlineIdentity = Identity.Get();
			FString name = Identity->GetPlayerNickname(*UniqueNetId);

			GetWorld()->ServerTravel("/Game/Maps/BloodGulch/BloodGulch?listen", true);		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("hm"));
	}

}

void USessionGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, bool bSearchLobbies)
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session && Session.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 1000;
			SessionSearch->PingBucketSize = 50;
			//SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, false, EOnlineComparisonOp::Equals);
			//SessionSearch->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
			//SessionSearch->QuerySettings.Set(SEARCH_SECURE_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);

			if (bSearchLobbies)
			{
				SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			}
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			}
			
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			
			UE_LOG(LogTemp, Display, TEXT("SessionInterface is valid. Finding sessions using provided settings..."));
			OnFindSessionsCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Session->FindSessions(0, SearchSettingsRef);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to initiate session searching due to invalid Session Interface!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to initiate session searching due to uninitialized Online Subsystem!"));
		OnFindSessionsComplete(false);
	}
}

void USessionGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("Finding sesssions %hs"), bWasSuccessful ? "succeeded" : "failed");

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session && Session.IsValid())
		{
			Session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			
			UE_LOG(LogTemp, Display, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			{
				UE_LOG(LogTemp, Display, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx+1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
				MenuWidgetHandle->CreateAndInsertSessionListingWidget(SessionSearch->SearchResults[SearchIdx], SESSION_NAME);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to retrieve queried sessions due to invalid Session Interface!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to retrieve queried sessions due to uninitialized Online Subsystem!"));
	}
	if (MenuWidgetHandle)
	{
		MenuWidgetHandle->RefreshButton->SetIsEnabled(true);
	}
}

bool USessionGameInstance::JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			//bSuccessful = Session->JoinSession(*UserId, SessionName, SearchResult);
			UE_LOG(LogTemp, Error, TEXT("Attemping to join session with SESSION_NAME: %s"), *SESSION_NAME.ToString());
			bSuccessful = Session->JoinSession(0, SESSION_NAME, SearchResult);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to initiate session joining due to invalid Session Interface or UserId!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to initiate session joining due to uninitialized Online Subsystem!"));
	}
	return bSuccessful;
}

void USessionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Display, TEXT("Attempt to join session with SessionName: %s has completed. Result: %s"), *SessionName.ToString(),  LexToString(Result));
	if (Result == EOnJoinSessionCompleteResult::Success ||
		Result == EOnJoinSessionCompleteResult::AlreadyInSession)
	{
		if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid())
			{
				if (FString URL; Session->GetResolvedConnectString(SessionName, URL))
				{
					UE_LOG(LogTemp, Display, TEXT("Preparing to client travel to destination URL: %s"), *URL);
					MenuWidgetHandle->GetOwningPlayer()->ClientTravel(URL, TRAVEL_Absolute);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Unable to resolve session connection string: %s"), *URL);
				}
				Session->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to join session due to invalid Session Interface!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to join session due to uninitialized Online Subsystem!"));
		}
	}
}

void USessionGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("Attempting to destroy session with SessionName: %s, has completed. Result: %hs"), *SessionName.ToString(),  bWasSuccessful ? "Successful" : "Failed");
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid())
		{
			Session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(MAIN_MENU_MAP_NAME), true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to destroy session due to invalid Session Interface!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to destroy session due to uninitialized Online Subsystem!"));
	}
}

void USessionGameInstance::StartOnlineGame(FName ServerName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, bool bUseLobbies)
{
	const ULocalPlayer* Player = GetFirstGamePlayer();
	const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	HostSession(UniqueNetId, SESSION_NAME, bIsLAN, bIsPresence, MaxNumPlayers, bUseLobbies);
}

void USessionGameInstance::FindOnlineGames(bool bIsLAN, bool bIsPresence, bool bSearchLobbies)
{
	const ULocalPlayer* Player = GetFirstGamePlayer();
	const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	FindSessions(UniqueNetId, bIsLAN, bIsPresence, bSearchLobbies);
}

void USessionGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(SESSION_NAME);
		}
	}
}

void USessionGameInstance::PopulateWidgetWithOnlineGames(USessionList* SessionListWidget)
{
	MenuWidgetHandle = SessionListWidget; // update once finished
	FindOnlineGames(SessionListWidget->LANCheckBox->IsChecked(), SessionListWidget->PresenceCheckBox->IsChecked(), false);
}

void USessionGameInstance::JoinOnlineGameProvidedSearchResult(FOnlineSessionSearchResult* SearchResult)
{
	const ULocalPlayer* Player = GetFirstGamePlayer();
	const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	JoinOnlineSession(0, SESSION_NAME, *SearchResult);
}

