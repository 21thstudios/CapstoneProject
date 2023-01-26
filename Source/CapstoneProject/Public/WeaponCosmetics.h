#pragma once

#include "CoreMinimal.h"
#include "WeaponCosmetics.generated.h"

UCLASS(BlueprintType)
class CAPSTONEPROJECT_API UWeaponCosmetics : public UObject
{
	GENERATED_BODY()

    // AttackSounds - Sound Base array
    // ReloadSounds - Sound Base array
    // AttackAnimations - Anim Sequence
    // ReloadAnimations - Anim Sequence
public:
	UWeaponCosmetics();
	UWeaponCosmetics(UAnimSequence* NewAttackAnimation, UAnimSequence* NewReloadAnimation);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAttackSounds, BlueprintSetter=SetAttackSounds)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAttackAnimation, BlueprintSetter=SetAttackAnimation)
	UAnimSequence* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetReloadAnimation, BlueprintSetter=SetReloadAnimation)
	UAnimSequence* ReloadAnimation;


	UFUNCTION(BlueprintCallable, BlueprintGetter)
    UAnimSequence* GetAttackAnimation();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetAttackAnimation(UAnimSequence* NewAttackAnimation);

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    UAnimSequence* GetReloadAnimation();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetReloadAnimation(UAnimSequence* NewReloadAnimation);
};
