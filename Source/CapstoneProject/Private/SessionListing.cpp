#include "SessionListing.h"

#include "SessionGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionListing::NativeConstruct()
{
    Super::NativeConstruct();
    JoinSessionButton->OnClicked.AddDynamic(this, &USessionListing::OnClickJoinSessionButton);
}

void USessionListing::NativeDestruct()
{
    Super::NativeDestruct();
    JoinSessionButton->OnClicked.RemoveDynamic(this, &USessionListing::OnClickJoinSessionButton);
}

void USessionListing::SetServerName(FText ServerName) const
{
    if (ServerNameTextBlock)
    {
        ServerNameTextBlock->SetText(ServerName);
    }
}

void USessionListing::SetPlayerCount(int32 CurrentPlayers, int32 MaxPlayers) const
{
    if (PlayerCountTextBlock) 
    {
        FString CurrentPlayersAsString = FString::FromInt(CurrentPlayers);
        FString MaxPlayersAsString = FString::FromInt(MaxPlayers);
        FString FormattedPlayerCount = CurrentPlayersAsString.Append("/").Append(MaxPlayersAsString);
        FText FormattedPlayersAsText = FText::FromString(FormattedPlayerCount);
        PlayerCountTextBlock->SetText(FormattedPlayersAsText);
    }
}

void USessionListing::SetPingMs(int32 PingInMs) const
{
    if (PingInMsTextBlock) 
    {
        FString FormattedPing = FString::FromInt(PingInMs).Append("ms");
        FText FormattedPingAsText = FText::FromString(FormattedPing);
        PingInMsTextBlock->SetText(FormattedPingAsText);        
    }
}

void USessionListing::OnClickJoinSessionButton()
{
    USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
    SessionGameInstance->JoinOnlineGameProvidedSearchResult(SessionResult);
}

void USessionListing::SetSessionResult(FOnlineSessionSearchResult* SessionSearchResult)
{
    this->SessionResult = SessionSearchResult;
}
