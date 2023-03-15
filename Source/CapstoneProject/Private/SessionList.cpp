#include "SessionList.h"

#include "OnlineSubsystemUtils.h"
#include "SessionGameInstance.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

void USessionList::NativeConstruct()
{
	Super::NativeConstruct();
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
	ClearSessionListings();
	RefreshButton->SetIsEnabled(false);
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	SessionGameInstance->PopulateWidgetWithOnlineGames(this);
}

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





