// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include "SessionGameInstance.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetMapName(FText::FromString(GetWorld()->GetMapName()));
	const USessionGameInstance* SessionGameInstance = static_cast<USessionGameInstance*>(GetGameInstance());
	SetServerName(FText::FromString(SessionGameInstance->HostedSessionInfo.ServerName.ToString()));
	// temp
	TArray<APlayerState*> LocalPlayerStates;
	LocalPlayerStates.Add(GetOwningPlayerState());
	UpdateEntries(LocalPlayerStates);
}

void UScoreboardWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UScoreboardWidget::SetMapName(FText MapName) const
{
	if (MapNameTextBlock)
	{
		FFormatNamedArguments Args;
		Args.Add("Prefix", FText::FromString("Map"));
		Args.Add("MapName", MapName);
		FText FormattedText = FText::Format(
			NSLOCTEXT("Scoreboard", "MapNameWithPrefixFormat", "{Prefix}: {MapName}"),
			Args);
		MapNameTextBlock->SetText(FormattedText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set map name of scoreboard due to invalid MapNameTextBlock!"));
	}
}

void UScoreboardWidget::SetServerName(FText ServerName) const
{
	if (ServerNameTextBlock)
	{
		FFormatNamedArguments Args;
		Args.Add("Prefix", FText::FromString("Server"));
		Args.Add("ServerName", ServerName);
		FText FormattedText = FText::Format(
			NSLOCTEXT("Scoreboard", "ServerNameWithPrefixFormat", "{Prefix}: {ServerName}"),
			Args);
		
		ServerNameTextBlock->SetText(FormattedText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set server name of scoreboard due to invalid ServerNameTextBlock!"));
	}
}

void UScoreboardWidget::AddEntry(const FUniqueNetId* UniqueNetId, FText DisplayName, int32 Ping, int32 NumKills,
	int32 NumDeaths)
{
	if (ScoreboardEntryScrollBox)
	{
		FName WidgetName = FName(UniqueNetId->ToString());
		UScoreboardEntryWidget* ScoreboardEntry = CreateWidget<UScoreboardEntryWidget>(this->GetOwningPlayer(), ScoreboardEntryBlueprintClass, WidgetName);
		ScoreboardEntry->SetNumDeaths(NumDeaths);
		ScoreboardEntry->SetNumKills(NumKills);
		ScoreboardEntry->SetPlayerDisplayName(DisplayName);
		ScoreboardEntry->SetPingInMs(Ping);
		InsertEntry(ScoreboardEntry);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add entry due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::InsertEntry(UScoreboardEntryWidget* ScoreboardEntryWidget)
{
	if (ScoreboardEntryScrollBox)
	{
		ScoreboardEntryScrollBox->AddChild(ScoreboardEntryWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to insert entry due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::UpdateEntries(TArray<APlayerState*> PlayerArray)
{
	if (ScoreboardEntryScrollBox)
	{
		ClearEntries();
		for (APlayerState* PlayerState : PlayerArray)
		{
			// todo update with Joey data
			int32 Ping = PlayerState->GetCompressedPing() * 4;
			int32 NumDeaths = 0;
			int32 NumKills = 0;
			FUniqueNetIdPtr UniqueNetId = PlayerState->GetUniqueId().GetV1();

			// temp
			IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
			IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
			FString name = Identity->GetPlayerNickname(*UniqueNetId);
			FText DisplayName = FText::FromString(name);
			
			AddEntry(UniqueNetId.Get(), DisplayName, Ping, NumKills, NumDeaths);
		}
		// temp
		int32 Ping = 0;
		int32 NumDeaths = 0;
		int32 NumKills = 0;
		FUniqueNetIdPtr UniqueNetId = GetOwningLocalPlayer()->GetPreferredUniqueNetId().GetV1();

		// temp
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
		FString name = Identity->GetPlayerNickname(*UniqueNetId);
		FText DisplayName = FText::FromString(name);
		AddEntry(UniqueNetId.Get(), DisplayName, Ping, NumKills, NumDeaths);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to update entries due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::ClearEntries()
{
	if (ScoreboardEntryScrollBox)
	{
		ScoreboardEntryScrollBox->ClearChildren();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to clear entries due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::ToggleViewport()
{
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		if (this->IsInViewport())
		{
			this->RemoveFromParent();
		}
		else
		{
			this->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController owning Scoreboard screen is not valid, therefore cannot pause/unpause"));
	}
}
