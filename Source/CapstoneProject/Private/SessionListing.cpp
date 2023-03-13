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
    std::cout << "Player count: " << CurrentPlayers << "/" << MaxPlayers << "\n";
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
