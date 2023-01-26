// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoState.generated.h"

USTRUCT(BlueprintType)
struct FAmmoState
{
	GENERATED_USTRUCT_BODY()

    FAmmoState()        // this is your default constructor which is required for a USTRUCT definition
    {
        // initialize things here if desired or required
    }

    UPROPERTY(BlueprintReadWrite)
    int32 SomeNumber;
};
