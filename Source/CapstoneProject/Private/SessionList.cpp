#include "SessionList.h"

#include "OnlineSubsystemUtils.h"
#include "SessionGameInstance.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBoxSlot.h"

void USessionList::NativeConstruct()
{
	Super::NativeConstruct();
	//OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionList::OnFindSessionsComplete);
	RefreshButton->OnClicked.AddDynamic(this, &USessionList::OnClickRefreshButton);
}

void USessionList::NativeDestruct()
{
	Super::NativeDestruct();
	RefreshButton->OnClicked.RemoveDynamic(this, &USessionList::OnClickRefreshButton);
}

void USessionList::SetLAN(bool bLAN)
{
	if (LANCheckBox)
	{
		LANCheckBox->SetIsChecked(bLAN);
	}
}

void USessionList::OnClickRefreshButton()
{
	UE_LOG(LogTemp, Display, TEXT("Pressed Refresh Button"));
	if (!SessionListingsScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionListingsScrollBox not initialized!!"));
		return;
	}
	// Clear previous search results from the Widget and temporarily make it unclickable for the duration of the search
	ClearSessionListings();
	RefreshButton->SetIsEnabled(false);
	
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	//SessionGameInstance->FindOnlineGames(LANCheckBox ? LANCheckBox->IsChecked() : false, true);
	SessionGameInstance->PopulateWidgetWithOnlineGames(this);
	/*
	// Define search terms and search asynchronously. Once finished, calls OnFindSessionsComplete which does the rest.
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session && Session.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			// todo add more confirable serach terms within options column
			SessionSearch->bIsLanQuery = LANCheckBox ? LANCheckBox->IsChecked() : false;
			SessionSearch->MaxSearchResults = 30000;
			SessionSearch->PingBucketSize = 50;
			//SessionSearch->QuerySettings.Set(SETTING_MAPNAME, FString("FirstPersonMap"), EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_SECURE_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
			
			//SessionSearch->TimeoutInSeconds
			if (bool bIsPresence = true)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			if (bool bSearchLobbies = true)
			{
				SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, bSearchLobbies, EOnlineComparisonOp::Equals);
			}
			
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			UE_LOG(LogTemp, Display, TEXT("Searching for multiplayer sessions"));
			OnFindSessionsCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Session->FindSessions(0, SearchSettingsRef);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Session or UniqueNetId is not valid!"));
			RefreshButton->SetIsEnabled(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
		OnFindSessionsComplete(false);
		RefreshButton->SetIsEnabled(true);
	}
	*/
}
/*
void USessionList::OnFindSessionsComplete(bool bWasSuccessful)
{
	// Search results are made as buttons and added to the SessionListingsScrollBox
	UE_LOG(LogTemp, Display, TEXT("OnFindSessionsComplete successfully handled. bWasSuccessful: %hs"), bWasSuccessful ? "true" : "false");
	if (!IsValid(WidgetSessionListingClass))
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetSessionListingClass is not initialized. Set it to a Widget blueprint child class from the SessionList blueprint"))
		return;
	}
	if (IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			// Clear the delegate handle now that we have finished the OnFindSessions call
			Session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			UE_LOG(LogTemp, Display, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());
			
			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			{
				UE_LOG(LogTemp, Display, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx+1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
				FName SessionName = FName((SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
				USessionListing* SessionListing = CreateWidget<USessionListing>(this->GetOwningPlayer(), WidgetSessionListingClass, SessionName);
				if (SessionListing)
				{

					// Create session listing, populate, and add to the ScrollBox
					FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[SearchIdx];
					SessionListing->SetSessionResult(&SearchResult);
					int32 const MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
					int32 const CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
					SessionListing->SetPlayerCount(CurrentPlayers, MaxPlayers);
					SessionListing->SetPingMs(SearchResult.PingInMs);
					SessionListing->SetServerName(FText::FromString(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
					this->AddSessionListing(SessionListing);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Search Result SessionListing pointer is not valid"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Session or UniqueNetId is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
	}
	RefreshButton->SetIsEnabled(true);
}
*/
USessionListing* USessionList::CreateAndInsertSessionListingWidget(FOnlineSessionSearchResult& SearchResult, FName SessionName)
{
	USessionListing* SessionListing = CreateWidget<USessionListing>(this->GetOwningPlayer(), WidgetSessionListingClass, SessionName);
	SessionListing->SetSessionResult(&SearchResult);
	int32 const MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
	int32 const CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
	SessionListing->SetPlayerCount(CurrentPlayers, MaxPlayers);
	SessionListing->SetPingMs(SearchResult.PingInMs);
	SessionListing->SetServerName(FText::FromString(SearchResult.Session.OwningUserName));
	this->AddSessionListing(SessionListing);
	return SessionListing;
}

void USessionList::ClearSessionListings()
{
	if (SessionListingsScrollBox)
	{
		SessionListingsScrollBox->ClearChildren();
	}
}

void USessionList::AddSessionListing(USessionListing* SessionListing)
{
	if (SessionListingsScrollBox)
	{
		SessionListingsScrollBox->AddChild(SessionListing);
	}
}





