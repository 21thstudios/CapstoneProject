#pragma once

#include "CoreMinimal.h"
#include "AmmoState.generated.h"

USTRUCT(BlueprintType)
struct FAmmoState
{
	GENERATED_BODY()

public:
    FAmmoState();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoCurrent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ClipSize;
};