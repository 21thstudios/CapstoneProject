
#include "WeaponCosmetics.h"
#include "../CapstoneProject.h"
#include "Animation/AnimSequence.h"

UWeaponCosmetics::UWeaponCosmetics()
{
}

UWeaponCosmetics::UWeaponCosmetics(UAnimSequence* NewAttackAnimation, UAnimSequence* NewReloadAnimation)
{
    // AttackSounds - Sound Base array
    // ReloadSounds - Sound Base array
    // AttackAnimation - Anim Sequence
    // ReloadAnimation - Anim Sequence
    this->AttackAnimation = NewAttackAnimation;
    this->ReloadAnimation = NewReloadAnimation;
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