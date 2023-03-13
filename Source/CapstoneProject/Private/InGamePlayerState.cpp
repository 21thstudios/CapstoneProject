// Fill out your copyright notice in the Description page of Project Settings.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "InGamePlayerState.h"

AInGamePlayerState::AInGamePlayerState()
{
    // According to S.O., superclass constructors 
    // are automatically called if they require no arguments. 
    // So, I think it's not necessary here. 
    this->PlayerKills = 0;
    this->PlayerDeaths = 0;
}

void AInGamePlayerState::HandleKill()
{
  
}

void AInGamePlayerState::ClearPlayerState()
{
    this->PlayerKills = 0;
    this->PlayerDeaths = 0;
}
