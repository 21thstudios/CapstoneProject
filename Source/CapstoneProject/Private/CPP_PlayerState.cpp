// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "CPP_PlayerState.h"
#include "CPP_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::FromInt(x));}
#define DFstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

ACPP_PlayerState::ACPP_PlayerState()
{
	this->Kills = 0;
	this->Deaths = 0;
	this->bReplicates = true;
}

void ACPP_PlayerState::ResetKillsAndDeaths()
{
	this->Kills = 0;
	this->Deaths = 0;
}

ACPP_PlayerState::~ACPP_PlayerState()
{
	
}

void ACPP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	this->Name = FName(this->GetPlayerName());
}

void ACPP_PlayerState::KillOtherPlayer(ACPP_PlayerState* OtherPlayer)
{
	this->Kills++;
	OtherPlayer->Deaths++;
	FString KillerName = this->Name.ToString();
	FString DeathName = OtherPlayer->Name.ToString();
	FString DisplayMessage = KillerName + " killed " + DeathName + "!";
	DFstr(DisplayMessage);
}

void ACPP_PlayerState::PrintKillsOnScreen()
{
	D("Current player's kills:");
	Dnum(this->Kills);
}

void ACPP_PlayerState::PrintDeathsOnScreen()
{
	D("Current player's deaths:");
	Dnum(this->Deaths);
}

void ACPP_PlayerState::PrintStatsOnScreen()
{
	this->PrintKillsOnScreen();
	this->PrintDeathsOnScreen();
}

void ACPP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_PlayerState, Kills);
	DOREPLIFETIME(ACPP_PlayerState, Deaths);
	DOREPLIFETIME(ACPP_PlayerState, Name);
}
