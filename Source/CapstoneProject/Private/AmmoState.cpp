#include "AmmoState.h"
#include "../CapstoneProject.h"

UAmmoState::UAmmoState()
{
    this->AmmoCurrent = 0;
    this->AmmoMax = 0;
    this->ClipSize = 0;
}

UAmmoState::UAmmoState(int32 NewAmmoCurrent, int32 NewAmmoMax, int32 NewClipSize)
{
    this->AmmoCurrent = NewAmmoCurrent;
    this->AmmoMax = NewAmmoMax;
    this->ClipSize = NewClipSize;
}

int32 UAmmoState::GetAmmoCurrent()
{
    return this->AmmoCurrent;
}

void UAmmoState::SetAmmoCurrent(int32 NewAmmoCurrent)
{
    this->AmmoCurrent = NewAmmoCurrent;
}

int32 UAmmoState::GetAmmoMax()
{
    return this->AmmoMax;
}

void UAmmoState::SetAmmoMax(int32 NewAmmoMax)
{
    this->AmmoMax = NewAmmoMax;
}

int32 UAmmoState::GetClipSize()
{
    return this->ClipSize;
}

void UAmmoState::SetClipSize(int32 NewClipSize)
{
    this->ClipSize = NewClipSize;
}
