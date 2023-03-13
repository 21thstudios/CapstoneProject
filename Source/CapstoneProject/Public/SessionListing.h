// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionListing.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class CAPSTONEPROJECT_API USessionListing : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetServerName(FText ServerName);

	void SetPlayerCount(int CurrentPlayers, int MaxPlayers);

	void SetPingMs(int PingInMs);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ServerNameTextBlock;
};
