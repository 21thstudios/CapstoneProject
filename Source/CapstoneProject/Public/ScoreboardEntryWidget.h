// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardEntryWidget.generated.h"

/**
 * Base user widget for scoreboard entries. This is the part of the scoreboard which contains individual player data. 
 */
UCLASS()
class CAPSTONEPROJECT_API UScoreboardEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void SetPlayerDisplayName(FText DisplayName);
	void SetPingInMs(int32 Ping);
	void SetNumKills(int32 NumKills);
	void SetNumDeaths(int32 NumDeaths);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerDisplayNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PingMsTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NumKillsTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NumDeathsTextBlock;
};
