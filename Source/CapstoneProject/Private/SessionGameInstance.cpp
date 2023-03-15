#include "SessionGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

const FName SESSION_NAME = FName(TEXT("TestSessionName"));
const FString MAIN_MENU_MAP_NAME = TEXT("MainMenuMap");
const FString HOST_MAP_DESTINATION_NAME = TEXT("FirstPersonMap");

USessionGameInstance::USessionGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnDestroySessionComplete);
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
	}
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(HOST_MAP_DESTINATION_NAME), true, "listen");
	}
}

void USessionGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session && Session.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 30000;
			SessionSearch->PingBucketSize = 50;
			SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_SECURE_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			
			UE_LOG(LogTemp, Display, TEXT("SessionInterface is valid. Finding sessions using provided settings..."));
			OnFindSessionsCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Session->FindSessions(*UserId, SearchSettingsRef);
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

	if (IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session && Session.IsValid())
		{
			Session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			
			UE_LOG(LogTemp, Display, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			{
				UE_LOG(LogTemp, Display, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx+1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
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
}

bool USessionGameInstance::JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			//bSuccessful = Session->JoinSession(*UserId, SessionName, SearchResult);
			bSuccessful = Session->JoinSession(*UserId, SESSION_NAME, SearchResult);
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
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface(); Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			APlayerController * const PlayerController = GetFirstLocalPlayerController();
			FString TravelURL;
			
			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				UE_LOG(LogTemp, Display, TEXT("Preparing to client travel to destination URL: %s"), *TravelURL);
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Unable to resolve session connection string: %s"), *TravelURL);
			}
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

void USessionGameInstance::StartOnlineGame(FName ServerName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	const ULocalPlayer* const Player = GetFirstGamePlayer();
	const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	HostSession(UniqueNetId, SESSION_NAME, bIsLAN, bIsPresence, MaxNumPlayers);
}

void USessionGameInstance::FindOnlineGames(bool bIsLAN, bool bIsPresence)
{
	const ULocalPlayer* const Player = GetFirstGamePlayer();
	const TSharedPtr<const FUniqueNetId> UniqueNetId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	FindSessions(UniqueNetId, bIsLAN, bIsPresence);
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