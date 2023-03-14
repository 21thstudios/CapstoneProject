#include "SessionListing.h"

#include "OnlineSubsystemUtils.h"
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
    IOnlineSubsystem *Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    
    if (JoinSessionButton)
    {
        const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
        const FUniqueNetId& UniqueNetId = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

        // Register the HandleJoinSessionComplete event handler
        this->JoinSessionDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(
            FOnJoinSessionComplete::FDelegate::CreateUObject(this, &USessionListing::HandleJoinSessionComplete));

        if (Session->JoinSession(UniqueNetId, SessionListingInfo.SessionName, *SessionListingInfo.SessionResult))
        {
            // Call successfully started 
        } else
        {
            // No longer associate the JoinSession delegate with the HandleJoinSessionComplete event
            Session->ClearOnJoinSessionCompleteDelegate_Handle(this->JoinSessionDelegateHandle);
            this->JoinSessionDelegateHandle.Reset();
        }
    }
}

/** JoinSession delegate fires this function once attempting to join a session completes */
void USessionListing::HandleJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult)
{
    const IOnlineSubsystem *Subsystem = Online::GetSubsystem(GetWorld());
    const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    
    if (JoinResult == EOnJoinSessionCompleteResult::Success ||
        JoinResult == EOnJoinSessionCompleteResult::AlreadyInSession)
    {
        // Travel to the server
        if (FString URL; Session->GetResolvedConnectString(SessionName, URL))
        {
            GetOwningPlayer()->ClientTravel(URL, TRAVEL_Absolute);
        }
    }
    // JoinSession Delegate should no longer be associated with this event
    Session->ClearOnJoinSessionCompleteDelegate_Handle(this->JoinSessionDelegateHandle);
    this->JoinSessionDelegateHandle.Reset();
}

void USessionListing::SetSessionName(FName SessionName)
{
    this->SessionListingInfo.SessionName = SessionName;
}

void USessionListing::SetOnlineSessionSearchResult(FOnlineSessionSearchResult* SearchResult)
{
    this->SessionListingInfo.SessionResult = SearchResult;
}
