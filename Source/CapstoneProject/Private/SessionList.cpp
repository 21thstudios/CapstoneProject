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
	// Empty the SessionListingsScrollBox
	ClearSessionListings();

	// Session-related info
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		const FUniqueNetId& UniqueNetId = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
		const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid() && UniqueNetId.IsValid())
		{
			// Retrieve all options from the options column
			bool bIsPresence = true;
			bool bShouldOnlySearchLAN = LANCheckBox ? LANCheckBox->IsChecked() : false;
			
			// Create a search with those options as filters
	
	

			// Search results are made as buttons and added to the SessionListingsScrollBox
		}
	}
}

void USessionList::OnFindSessionsComplete(bool bWasSuccessful)
{
	
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





