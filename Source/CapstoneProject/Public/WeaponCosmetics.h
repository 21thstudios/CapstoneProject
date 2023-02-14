#pragma once

#include "CoreMinimal.h"
#include "WeaponCosmetics.generated.h"

USTRUCT(BlueprintType)
struct FWeaponCosmetics
{
	GENERATED_BODY()

public:
	FWeaponCosmetics();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> AttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> ReloadSounds;
};