// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardEntryWidget.h"

#include "Components/TextBlock.h"

void UScoreboardEntryWidget::SetPlayerDisplayName(FText DisplayName) const
{
	if (PlayerDisplayNameTextBlock)
	{
		PlayerDisplayNameTextBlock->SetText(DisplayName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set name of scoreboard entry due to invalid PlayerDisplayNameTextBlock!"));
	}
}

void UScoreboardEntryWidget::SetPingInMs(int32 Ping) const
{
	if (PingMsTextBlock)
	{
		FFormatNamedArguments Args;
		Args.Add("PingInMs", Ping);
		Args.Add("ms", *TEXT("ms"));
		FText FormattedText = FText::Format(
			NSLOCTEXT("ScoreboardEntry", "PingInMsFormat", "{PingInMs}{ms}"),
			Args);
		PingMsTextBlock->SetText(FormattedText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set ping of scoreboard entry due to invalid PingMsTextBlock!"));
	}
}

void UScoreboardEntryWidget::SetNumKills(int32 NumKills) const
{
	if (NumKillsTextBlock)
	{
		FText NumKillsParsed = FText::FromString(FString::FromInt(NumKills));
		NumKillsTextBlock->SetText(NumKillsParsed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set kills of scoreboard entry due to invalid NumKillsTextBlock!"));
	}
}
