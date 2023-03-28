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
		UE_LOG(LogTemp, Error, TEXT("Failed to set server name of scoreboard entry due to invalid PlayerDisplayNameTextBlock!"));
	}
}
