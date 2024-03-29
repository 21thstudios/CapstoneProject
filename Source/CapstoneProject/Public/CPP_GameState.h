// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

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
  FTimerHandle GameEndTimer;
  FTimerDelegate Delegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int kills_to_end;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameEndTimeInSeconds;
	
	UFUNCTION()
	void ResetStateForNewGame();

	void ResetAllPlayersStates();

	UFUNCTION(BlueprintCallable)
	float GetSecondsRemainingOfGame() const;

	UFUNCTION(BlueprintCallable)
	FText GetFormattedTimeRemainingOfGame();

  bool ShouldEndGameByTime();

  bool ShouldEndGameByKills();

  void HandleGameEndByKills();

  void HandleGameEndByTime();

  void HandleGameEnd();
	

	UFUNCTION()
	void BeginPlay();
};
