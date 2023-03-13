#include "SessionList.h"

#include "OnlineSubsystemUtils.h"

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





