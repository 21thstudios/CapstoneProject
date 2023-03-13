#include "SessionList.h"

#include "OnlineSubsystemUtils.h"

void USessionList::SetLAN(bool LAN)
{
	if (LANCheckBox)
	{
		LANCheckBox->SetIsChecked(LAN);
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
	const IOnlineSubsystem *Subsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const FUniqueNetId& UniqueNetId = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
	
	// Retrieve all options from the options column
	bool IsPresence = true;
	bool ShouldOnlySearchLAN = LANCheckBox ? LANCheckBox->IsChecked() : false;
	
	
	// Create a search with those options as filters
	

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





