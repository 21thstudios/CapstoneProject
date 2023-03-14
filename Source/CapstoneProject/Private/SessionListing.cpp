#include "SessionListing.h"

#include "OnlineSubsystemUtils.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionListing::NativeConstruct()
{
    Super::NativeConstruct();
    JoinSessionButton->OnClicked.AddDynamic(this, &USessionListing::OnClickJoinSessionButton);
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionListing::OnJoinSessionComplete);
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
    if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
    {
        if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid())
        {
            // Register the HandleJoinSessionComplete event handler
            Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
            FName SessionName = ServerNameTextBlock ? FName(ServerNameTextBlock->GetText().ToString()) : FName(TEXT("Unnamed Server"));
            FOnlineSessionSearchResult SessionSearchResult = *SessionResult;

            if (Session->JoinSession(0, SessionName, SessionSearchResult))
            {
                UE_LOG(LogTemp, Display, TEXT("Session successfully dispatched the join session %s call with name"), *SessionName.ToString());
            } else
            {
                UE_LOG(LogTemp, Display, TEXT("Session failed to dispatch the join session %s call"), *SessionName.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Session is not valid!"));
            JoinSessionButton->SetIsEnabled(true);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
        JoinSessionButton->SetIsEnabled(true);
    }
}

/** JoinSession delegate fires this function once attempting to join a session completes */
void USessionListing::OnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult)
{
    UE_LOG(LogTemp, Display, TEXT("OnJoinSessionComplete delegate handled. JoinResult: %s"), LexToString(JoinResult));
    if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
    {
        if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface(); Session.IsValid())
        {
            if (JoinResult == EOnJoinSessionCompleteResult::Success ||
                JoinResult == EOnJoinSessionCompleteResult::AlreadyInSession)
            {
                if (FString URL; Session->GetResolvedConnectString(SessionName, URL))
                {
                    UE_LOG(LogTemp, Display, TEXT("Traveling player to server with URL: %s"), *URL);
                    GetOwningPlayer()->ClientTravel(URL, TRAVEL_Absolute);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Unable to resolve session connection string: %s"), *URL);
                }
            }
            Session->ClearOnJoinSessionCompleteDelegate_Handle(this->OnJoinSessionDelegateHandle);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Session is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
    }
    JoinSessionButton->SetIsEnabled(true);
}

void USessionListing::SetSessionResult(FOnlineSessionSearchResult* SessionSearchResult)
{
    this->SessionResult = SessionSearchResult;
}
