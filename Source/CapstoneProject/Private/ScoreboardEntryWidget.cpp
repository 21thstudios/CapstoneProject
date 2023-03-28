// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardEntryWidget.h"

#include "Components/TextBlock.h"

void UScoreboardEntryWidget::SetPlayerDisplayName(FText DisplayName)
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

void UScoreboardEntryWidget::SetPingInMs(int32 Ping)
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
