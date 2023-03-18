// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API ACPP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACPP_PlayerState();
	virtual ~ACPP_PlayerState();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int Kills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int Deaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UFUNCTION(BlueprintCallable)
	void ResetKillsAndDeaths();

	UFUNCTION(BlueprintCallable)
	void KillOtherPlayer(ACPP_PlayerState* OtherPlayer);

	void BeginPlay() override;

	void PrintKillsOnScreen();

	void PrintDeathsOnScreen();

	void PrintStatsOnScreen();
};
