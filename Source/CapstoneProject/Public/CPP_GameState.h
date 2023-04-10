// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

// #include <chrono>
// #include <string>
#include "Misc/DateTime.h"
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
	int time_at_start;
  FTimerHandle GameEndTimer;
  FTimerDelegate Delegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int kills_to_end;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int time_to_end;
	
	UFUNCTION()
	void ResetStateForNewGame();

	void ResetAllPlayersStates();

	void SetGameStartTimeToNow();

  bool ShouldEndGameByTime();

  bool ShouldEndGameByKills();

  void HandleGameEndByKills();

  void HandleGameEndByTime();

  int GetTimeSinceGameStart();

  void HandleGameEnd();

  int StartTime();

	UFUNCTION()
	void BeginPlay();
};
