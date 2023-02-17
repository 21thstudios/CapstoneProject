// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.generated.h"

UCLASS(BlueprintType)
/**
 * 
 */
class CAPSTONEPROJECT_API UPlayerStats : public UObject
{
  GENERATED_BODY()

public:
	UPlayerStats();
	~UPlayerStats();

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int PlayerKills;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int PlayerDeaths;

  UFUNCTION(BlueprintCallable)
  void AddKill();
  UFUNCTION(BlueprintCallable)
  void AddDeath();
  UFUNCTION(BlueprintCallable)
  void HandleDeath(UPlayerStats* Actor);

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
