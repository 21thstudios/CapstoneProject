#include "SessionList.h"

#include "OnlineSubsystemUtils.h"
#include "Components/Button.h"

void USessionList::NativeConstruct()
{
	Super::NativeConstruct();
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionList::OnFindSessionsComplete);
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
	// todo check refersh button looks pressed
	//RefreshButton->SetStyle(RefreshButton->WidgetStyle.Pressed);
	//RefreshButton->WidgetStyle.SetPressed(new FSlateBrush());

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
}

void USessionList::OnFindSessionsComplete(bool bWasSuccessful)
{
	// Search results are made as buttons and added to the SessionListingsScrollBox
	UE_LOG(LogTemp, Display, TEXT("OnFindSessionsComplete successfully handled. bWasSuccessful: %hs"), bWasSuccessful ? "true" : "false");

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
					//FSessionListingInfo SessionListingInfo = 
					FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[SearchIdx];
					SessionListing->SetOnlineSessionSearchResult(&SearchResult);
					SessionListing->SetSessionName(FName((SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
					int32 const MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
					int32 const CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
					SessionListing->SetPlayerCount(CurrentPlayers, MaxPlayers);
					SessionListing->SetPingMs(SearchResult.PingInMs);
					SessionListing->SetServerName(FText::FromString(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
					AddSessionListing(SessionListing);
				}
			}
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
		RefreshButton->SetIsEnabled(true);
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





