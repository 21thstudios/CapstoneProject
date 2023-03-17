// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "CPP_PlayerState.h"
#include "CPP_GameState.h"
#include "Engine/World.h"
#include <string>
#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(x).c_str());}

ACPP_PlayerState::ACPP_PlayerState()
{
	this->Kills = 0;
	this->Deaths = 0;
	this->Name = "a";
	D("scree");
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
	GetWorld()->GetGameState<ACPP_GameState>()->ResetAllPlayersStates();
}

void ACPP_PlayerState::KillOtherPlayer(ACPP_PlayerState* OtherPlayer)
{
	this->Kills++;
	OtherPlayer->Deaths++;
	this->PrintStatsOnScreen();
	OtherPlayer->PrintStatsOnScreen();
	
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



