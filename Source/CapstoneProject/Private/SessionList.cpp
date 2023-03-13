#include "SessionList.h"

void USessionList::SetLAN(bool LAN)
{
	if (LANCheckBox)
	{
		LANCheckBox->SetIsChecked(LAN);
	}
}

void USessionList::OnClickRefreshButton()
{
	// Empty the SessionListingsScrollBox
	
	// Retrieve all options from the options column

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


