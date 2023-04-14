// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include <chrono>

#include "CPP_PlayerState.h"
#include "SessionGameInstance.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ACPP_PlayerState* PlayerState = static_cast<ACPP_PlayerState*>(GetOwningPlayerState());
	PlayerState->OnUpdateEntriesScoreboardDelegate.AddDynamic(this, &UScoreboardWidget::OnUpdateEntries);
	OnUpdateEntries(PlayerState->ScoreboardData);
}

void UScoreboardWidget::NativeDestruct()
{
	Super::NativeDestruct();
	ACPP_PlayerState* PlayerState = static_cast<ACPP_PlayerState*>(GetOwningPlayerState());
	PlayerState->OnUpdateEntriesScoreboardDelegate.RemoveDynamic(this, &UScoreboardWidget::OnUpdateEntries);
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

void UScoreboardWidget::OnUpdateEntries(FScoreboardData ScoreboardData)
{
	if (ScoreboardEntryScrollBox)
	{
		SetMapName(FText::FromString(GetWorld()->GetMapName()));
		SetServerName(ScoreboardData.ServerName);
		SetRemainingTimeInSeconds(ScoreboardData.SecondsRemainingOfGame);
		ClearEntries(); // todo pre-build all widgets, then clear, then insert all the entries
		for (FScoreboardEntryData ScoreboardEntryData : ScoreboardData.ScoreboardEntryData)
		{
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
