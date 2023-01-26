#include "DamageProps.h"
#include "../CapstoneProject.h"

UDamageProps::UDamageProps()
{
    this->FireRate = 0.0;
    this->Damage = 0.0;
    this->IsAutomatic = false;
}

UDamageProps::UDamageProps(float NewDamage, float NewFireRate, bool NewIsAutomatic)
{
    this->FireRate = NewFireRate;
    this->Damage = NewDamage;
    this->IsAutomatic = NewIsAutomatic;
}

float UDamageProps::GetDamage()
{
    return this->Damage;
}

void UDamageProps::SetDamage(float NewDamage)
{
    this->Damage = NewDamage;
}

float UDamageProps::GetFireRate()
{
    return this->FireRate;
}

void UDamageProps::SetFireRate(float NewFireRate)
{
    this->FireRate = NewFireRate;
}

bool UDamageProps::GetIsAutomatic()
{
    return this->IsAutomatic;
}

void UDamageProps::SetIsAutomatic(bool NewIsAutomatic)
{
    this->IsAutomatic = NewIsAutomatic;
}