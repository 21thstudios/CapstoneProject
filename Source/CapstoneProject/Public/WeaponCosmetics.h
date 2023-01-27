#pragma once

#include "CoreMinimal.h"
#include "WeaponCosmetics.generated.h"

UCLASS(BlueprintType)
class CAPSTONEPROJECT_API UWeaponCosmetics : public UObject
{
	GENERATED_BODY()

public:
	UWeaponCosmetics();
	UWeaponCosmetics(UAnimSequence* NewAttackAnimation, UAnimSequence* NewReloadAnimation, TArray<USoundBase*> NewAttackSounds, TArray<USoundBase*> NewReloadSounds);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAttackAnimation, BlueprintSetter=SetAttackAnimation)
	UAnimSequence* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetReloadAnimation, BlueprintSetter=SetReloadAnimation)
	UAnimSequence* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAttackSounds, BlueprintSetter=SetAttackSounds)
	TArray<USoundBase*> AttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetReloadSounds, BlueprintSetter=SetReloadSounds)
	TArray<USoundBase*> ReloadSounds;

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    UAnimSequence* GetAttackAnimation();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetAttackAnimation(UAnimSequence* NewAttackAnimation);

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    UAnimSequence* GetReloadAnimation();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetReloadAnimation(UAnimSequence* NewReloadAnimation);

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    TArray<USoundBase*> GetAttackSounds();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetAttackSounds(TArray<USoundBase*> NewAttackSounds);

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    TArray<USoundBase*> GetReloadSounds();

	UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetReloadSounds(TArray<USoundBase*> NewReloadSounds);
};
