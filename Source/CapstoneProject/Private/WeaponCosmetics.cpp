
#include "WeaponCosmetics.h"
#include "../CapstoneProject.h"
#include "Animation/AnimSequence.h"
#include "Sound/SoundBase.h"

UWeaponCosmetics::UWeaponCosmetics()
{
}

UWeaponCosmetics::UWeaponCosmetics(UAnimSequence* NewAttackAnimation, UAnimSequence* NewReloadAnimation, TArray<USoundBase*> NewAttackSounds, TArray<USoundBase*> NewReloadSounds)
{
    this->AttackAnimation = NewAttackAnimation;
    this->ReloadAnimation = NewReloadAnimation;
    this->AttackSounds = NewAttackSounds;
    this->ReloadSounds = NewReloadSounds;
}

UAnimSequence* UWeaponCosmetics::GetAttackAnimation()
{
    return this->AttackAnimation;
}


void UWeaponCosmetics::SetAttackAnimation(UAnimSequence* NewAttackAnimation)
{
    this->AttackAnimation = NewAttackAnimation;
}

UAnimSequence* UWeaponCosmetics::GetReloadAnimation()
{
    return this->ReloadAnimation;
}

void UWeaponCosmetics::SetReloadAnimation(UAnimSequence* NewReloadAnimation)
{
    this->ReloadAnimation = NewReloadAnimation;
}

TArray<USoundBase*> UWeaponCosmetics::GetAttackSounds()
{
    return this->AttackSounds;
}

void UWeaponCosmetics::SetAttackSounds(TArray<USoundBase*> NewAttackSounds)
{
    this->AttackSounds = NewAttackSounds;
}

TArray<USoundBase*> UWeaponCosmetics::GetReloadSounds()
{
    return this->ReloadSounds;
}

void UWeaponCosmetics::SetReloadSounds(TArray<USoundBase*> NewReloadSounds)
{
    this->ReloadSounds = NewReloadSounds;
}