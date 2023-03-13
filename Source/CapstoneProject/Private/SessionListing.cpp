#include "SessionListing.h"
#include <iostream>
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void USessionListing::SetServerName(FText ServerName)
{
    if (ServerNameTextBlock)
    {
        ServerNameTextBlock->SetText(ServerName);
    }
}

void USessionListing::SetPlayerCount(int32 CurrentPlayers, int32 MaxPlayers)
{
    if (PlayerCountTextBlock) {
        FString CurrentPlayersAsString = FString::FromInt(CurrentPlayers);
        FString MaxPlayersAsString = FString::FromInt(MaxPlayers);
        FString FormattedPlayerCount = CurrentPlayersAsString.Append("/").Append(MaxPlayersAsString);
        FText FormattedPlayersAsText = FText::FromString(FormattedPlayerCount);
        PlayerCountTextBlock->SetText(FormattedPlayersAsText);
    }
}

void USessionListing::SetPingMs(int32 PingInMs)
{
    FString FormattedPing = FString::FromInt(PingInMs).Append("ms");
    FText FormattedPingAsText = FText::FromString(FormattedPing);
    if (PingInMsTextBlock)
    {
        PingInMsTextBlock->SetText(FormattedPingAsText);
    }
}
