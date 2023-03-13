// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int PlayerKills;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int PlayerDeaths;

    UFUNCTION(BlueprintCallable)
    void HandleKill();

    UFUNCTION(BlueprintCallable)
    void ClearPlayerState();

    UFUNCTION(BlueprintCallable)
    void AddKill();
    UFUNCTION(BlueprintCallable)
    void AddDeath();

    // getter and setter for each 
    UFUNCTION(BlueprintCallable)
    int GetPlayerKills();
    UFUNCTION(BlueprintCallable)
    int GetPlayerDeaths();
    UFUNCTION(BlueprintCallable)
    void SetPlayerKills(int Kills);
    UFUNCTION(BlueprintCallable)
    void SetPlayerDeaths(int Deaths);
};
