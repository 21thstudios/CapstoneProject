// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include <chrono>

#include "CPP_GameState.h"
#include "CPP_PlayerState.h"
#include "SessionGameInstance.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// todo game state should call OnUpdateEntries 
	const USessionGameInstance* SessionGameInstance = static_cast<USessionGameInstance*>(GetGameInstance());
	TArray<FScoreboardEntryData*> ScoreboardEntryDataArray;
	FScoreboardEntryData ScoreboardEntryData;
	FUniqueNetIdPtr UniqueNetId = GetOwningPlayerState()->GetUniqueId().GetV1();
	
	// Retrieve the Steam username. We should cache this on the server
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
	FString name = Identity->GetPlayerNickname(*UniqueNetId);
	FText DisplayName = FText::FromString(name);

	// Populate local player scoreboard entry. This should be managed by server
	ScoreboardEntryData.NumDeaths = 0;
	ScoreboardEntryData.NumKills = 0;
	ScoreboardEntryData.PingInMillis = GetOwningPlayerState()->GetCompressedPing() * 4;
	ScoreboardEntryData.SteamDisplayName = DisplayName;
	ScoreboardEntryData.UniqueNetId = UniqueNetId.Get();
	ScoreboardEntryDataArray.Add(&ScoreboardEntryData);
	
	FScoreboardData ScoreboardData;
	ScoreboardData.MapName = FText::FromString(GetWorld()->GetMapName());
	ScoreboardData.ServerName = FText::FromString(SessionGameInstance->HostedSessionInfo.ServerName.ToString());
	ScoreboardData.SecondsRemainingOfGame = 125;
	ScoreboardData.ScoreboardEntryData = ScoreboardEntryDataArray;
	SetServerName(FText::FromString(SessionGameInstance->HostedSessionInfo.ServerName.ToString()));
	SetMapName(FText::FromString(GetWorld()->GetMapName()));
	ACPP_GameState* GameState = static_cast<ACPP_GameState*>(UGameplayStatics::GetGameState(GetWorld()));
	SetRemainingTimeInSeconds(GameState->StartTime() + GameState->time_to_end - FDateTime::Now().ToUnixTimestamp());
	RefreshScoreboardEntries();
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

void UScoreboardWidget::SetRemainingTimeInSeconds(int32 RemainingTimeSeconds)
{
	if (RemainingTimeSecondsTextBlock)
	{
		int32 RemainingTimeMinutes = RemainingTimeSeconds / 60;
		int32 LeftoverSeconds = RemainingTimeSeconds % 60;
		FString RemainingTimeMinutesString = RemainingTimeMinutes > 0 ? FString::FromInt(RemainingTimeMinutes) : "";
		FString RemainingTimeSecondsString = RemainingTimeMinutes > 0 && LeftoverSeconds < 10 ? FString("0").Append(FString::FromInt(LeftoverSeconds)) : FString::FromInt(LeftoverSeconds);
		
		FFormatNamedArguments Args;
		Args.Add("MinutesRemaining", FText::FromString(RemainingTimeMinutesString));
		Args.Add("MinutesRemainingDelimiter", RemainingTimeMinutes > 0 ? FText::FromString(":") : FText::FromString(""));
		Args.Add("SecondsRemaining", FText::FromString(RemainingTimeSecondsString));
		FText FormattedText = FText::Format(
	NSLOCTEXT("Scoreboard", "RemainingTimeFormat", "{MinutesRemaining}{MinutesRemainingDelimiter}{SecondsRemaining}"), Args);
		RemainingTimeSecondsTextBlock->SetText(FormattedText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add entry due to invalid RemainingTimeSecondsTextBlock!"));
	}
}

void UScoreboardWidget::AddEntry(FScoreboardEntryData* ScoreboardEntryData)
{
	if (ScoreboardEntryScrollBox)
	{
		const FName WidgetName = FName(ScoreboardEntryData->UniqueNetId->ToString());
		UScoreboardEntryWidget* ScoreboardEntry = CreateWidget<UScoreboardEntryWidget>(this->GetOwningPlayer(), ScoreboardEntryBlueprintClass, WidgetName);
		ScoreboardEntry->SetAll(ScoreboardEntryData);
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

void UScoreboardWidget::RefreshScoreboardEntries()
{
	if (ScoreboardEntryScrollBox)
	{
		ClearEntries();
		TArray<TObjectPtr<APlayerState>> PlayerArray = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
		
		for (APlayerState* PS : PlayerArray)
		{
			ACPP_PlayerState* PlayerState = static_cast<ACPP_PlayerState*>(PS);
			FScoreboardEntryData ScoreboardEntryData = FScoreboardEntryData();
			ScoreboardEntryData.NumDeaths = PlayerState->Deaths;
			ScoreboardEntryData.NumKills = PlayerState->Kills;
			ScoreboardEntryData.PingInMillis = PlayerState->GetCompressedPing() * 4;
			ScoreboardEntryData.UniqueNetId = PlayerState->GetUniqueId().GetV1().Get();
			ScoreboardEntryData.SteamDisplayName = FText::FromString(Identity->GetPlayerNickname(*PlayerState->GetUniqueId().GetV1()));
			
			AddEntry(&ScoreboardEntryData);
		}
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

void UScoreboardWidget::ToggleViewport(bool ShouldAddToViewport)
{
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		if (this->IsInViewport() && !ShouldAddToViewport)
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
