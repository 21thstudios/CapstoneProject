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
	UFUNCTION()
	void ResetStateForNewGame();

	void ResetAllPlayersStates();

	void SetGameStartTimeToNow();
};
