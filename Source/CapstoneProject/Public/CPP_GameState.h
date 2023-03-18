// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include <chrono>
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPP_GameState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API ACPP_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACPP_GameState();

private:
	std::chrono::milliseconds time_at_start;

public:
	// Was unable to complete this part: 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// Haven't looked into C++ enums here, but would be a better solution.
	// If `mode` is equal to `kills`, ends once the correct number of kills is reached.
	// If `mode` is equal to `time`, ends once the correct amount of time has passed.
	// I'm curious about other coding solutions here-- I would hesitate to use inheritance
	// because it's not flexible, instead preferring composition. But then, using a switch
	// statement would be messy. So maybe we could define an interface for where to check
	// if the game should end, but that might have to check every tick, because otherwise
	// we'll need to check in multiple different spots for each condition. Further,
	// we might want to add another condition in the future for the game to end or use a
	// combination of conditions. 
	// string mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int time_or_kills_to_end;
	
	UFUNCTION()
	void ResetStateForNewGame();

	void ResetAllPlayersStates();

	void SetGameStartTimeToNow();

	UFUNCTION()
	void BeginPlay();
};
