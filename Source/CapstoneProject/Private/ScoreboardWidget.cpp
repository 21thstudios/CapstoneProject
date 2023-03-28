// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include "Components/TextBlock.h"

void UScoreboardWidget::SetMapName(FText MapName) const
{
	if (MapNameTextBlock)
	{
		MapNameTextBlock->SetText(MapName);
	}
}

void UScoreboardWidget::SetServerName(FText ServerName) const
{
	if (ServerNameTextBlock)
	{
		FFormatNamedArguments Args;
		Args.Add("Prefix", *TEXT("Server"));
		Args.Add("ServerName", ServerName);
		FText FormattedText = FText::Format(
			NSLOCTEXT("Scoreboard", "ServerNameWithPrefixFormat", "{Prefix}: {ServerName}"),
			Args);
		
		ServerNameTextBlock->SetText(FormattedText);
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to set server name of scoreboard due to invalid ServerNameTextBlock!"));
}
