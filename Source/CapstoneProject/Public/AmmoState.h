#pragma once

#include "CoreMinimal.h"
#include "AmmoState.generated.h"

UCLASS(BlueprintType)
class UAmmoState : public UObject
{
	GENERATED_BODY()

public:
    UAmmoState();
    UAmmoState(int32 NewAmmoCurrent, int32 NewAmmoMax, int32 NewClipSize);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAmmoCurrent, BlueprintSetter=SetAmmoCurrent)
    int32 AmmoCurrent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetAmmoMax, BlueprintSetter=SetAmmoMax)
    int32 AmmoMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetClipSize, BlueprintSetter=SetClipSize)
    int32 ClipSize;

    UFUNCTION(BlueprintCallable, BlueprintGetter)
    int32 GetAmmoCurrent();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetAmmoCurrent(int32 NewAmmoCurrent);

    UFUNCTION(BlueprintCallable, BlueprintGetter)
    int32 GetAmmoMax();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetAmmoMax(int32 NewAmmoMax);

    UFUNCTION(BlueprintCallable, BlueprintGetter)
    int32 GetClipSize();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetClipSize(int32 NewClipSize);
};