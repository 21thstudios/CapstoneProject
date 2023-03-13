#include "SessionList.h"

#include "OnlineSubsystemUtils.h"

USessionList::USessionList()
{
	// todo investigate cleanup
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionList::OnFindSessionsComplete);
}

USessionList::~USessionList()
{
	if (RefreshButton && !RefreshButton->OnClicked.IsBound())
	{
		RefreshButton->OnClicked.RemoveDynamic(this, &USessionList::OnClickRefreshButton);
	}
}


void USessionList::BindOnClicked()
{
	if (RefreshButton && !RefreshButton->OnClicked.IsBound())
	{
		RefreshButton->OnClicked.AddDynamic(this, &USessionList::OnClickRefreshButton);
	}
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
	if (!SessionListingsScrollBox)
	{
		return;
	}
	// Clear previous search results from the Widget
	ClearSessionListings();

	// Define search terms and search asynchronously. Once finished, calls OnFindSessionsComplete which does the rest.
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		const FUniqueNetId& UniqueNetId = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
		const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid() && UniqueNetId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			// todo add more confirable serach terms within options column
			SessionSearch->bIsLanQuery = LANCheckBox ? LANCheckBox->IsChecked() : false;
			SessionSearch->MaxSearchResults = 25;
			SessionSearch->PingBucketSize = 50;
			
			bool bIsPresence = true;
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			
			OnFindSessionsCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void USessionList::OnFindSessionsComplete(bool bWasSuccessful)
{
	// Search results are made as buttons and added to the SessionListingsScrollBox
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	if (IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			// Clear the delegate handle now that we have finished the OnFindSessions call
			Session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx+1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
					USessionListing* SessionListing = NewObject<USessionListing>(this, USessionListing::StaticClass());

					// Create session listing, populate, and add to the ScrollBox
					FOnlineSessionSearchResult const SearchResult = SessionSearch->SearchResults[SearchIdx];
					int32 const MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
					int32 const CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
					SessionListing->SetPlayerCount(CurrentPlayers, MaxPlayers);
					SessionListing->SetPingMs(SearchResult.PingInMs);
					SessionListing->SetServerName(FText::FromString(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
					AddSessionListing(SessionListing);
				}
			}
		}
	}
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
	if (SessionListingsScrollBox && SessionListing)
	{
		SessionListingsScrollBox->AddChild(SessionListingsScrollBox);
	}
}





