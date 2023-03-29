// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScoreboardEntryWidget.generated.h"

USTRUCT(BlueprintType)
struct FScoreboardEntryData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SteamDisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PingInMillis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumKills;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumDeaths;
};

/**
 * Base user widget for scoreboard entries. This is the part of the scoreboard which contains individual player data. 
 */
UCLASS()
class CAPSTONEPROJECT_API UScoreboardEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerDisplayName(FText DisplayName) const;
	void SetPingInMs(int32 Ping) const;
	void SetNumKills(int32 NumKills) const;
	void SetNumDeaths(int32 NumDeaths) const;
	void SetAll(FScoreboardEntryData* ScoreboardEntryData) const;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerDisplayNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PingMsTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NumKillsTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NumDeathsTextBlock;
};
