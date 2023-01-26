#pragma once

#include "CoreMinimal.h"
#include "DamageProps.generated.h"

UCLASS(BlueprintType)
class UDamageProps : public UObject
{
	GENERATED_BODY()

public:
	UDamageProps();
    UDamageProps(float NewDamage, float NewFireRate, bool NewIsAutomatic);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetDamage, BlueprintSetter=SetDamage)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetFireRate, BlueprintSetter=SetFireRate)
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetIsAutomatic, BlueprintSetter=SetIsAutomatic)
    bool IsAutomatic;

	UFUNCTION(BlueprintCallable, BlueprintGetter)
    float GetDamage();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetDamage(float NewDamage);

    UFUNCTION(BlueprintCallable, BlueprintGetter)
    float GetFireRate();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetFireRate(float NewFireRate);

    UFUNCTION(BlueprintCallable, BlueprintGetter)
    bool GetIsAutomatic();

    UFUNCTION(BlueprintCallable, BlueprintSetter)
    void SetIsAutomatic(bool NewIsAutomatic);
};