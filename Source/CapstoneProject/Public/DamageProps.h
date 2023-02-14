#pragma once

#include "CoreMinimal.h"
#include "DamageProps.generated.h"

USTRUCT(BlueprintType)
struct FDamageProps
{
	GENERATED_BODY()

public:
	FDamageProps();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsAutomatic;
};