// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "CPP_PlayerState.h"
#include "CPP_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include <string>
#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(x).c_str());}

ACPP_PlayerState::ACPP_PlayerState()
{
	// Inheriting here and adding instance variables feels not modular. Any recommendations
	// for whether to continue doing it this way, or maybe extract these variables into another
	// class/some other approach? UE encourages doing it this way. 
	this->Kills = 0;
	this->Deaths = 0;
	// Temporary solution here-- we would need to extend our API to get a proper player name,
	// so we're using the player ID in string form. 
	this->Name = std::to_string(this->GetPlayerId()).c_str();
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
	GetWorld()->GetGameState<ACPP_GameState>()->ResetAllPlayersStates();
	
}

void ACPP_PlayerState::KillOtherPlayer(ACPP_PlayerState* OtherPlayer)
{
	this->Kills++;
	OtherPlayer->Deaths++;
	// Debug message to ensure that the player killing and the player being killed
	// are not the same. 
	if (OtherPlayer == this)
	{
		D("OtherPlayer is self");
	}
	// More debug. 
	// this->PrintStatsOnScreen();
	// OtherPlayer->PrintStatsOnScreen();

	ACPP_GameState* GameState = GetWorld()->GetGameState<ACPP_GameState>();
	// const char* mode = GameState->mode; 
	// If we're playing in the mode where the game ends once a certain number of kills
	// are reached, execute this code (definitely not an ideal solution). It also feels
	// weird to grab a `GameState` reference from a `PlayerState`. I don't see anything
	// immediately wrong with it, but I'm curious for any advice/suggestions. 
	// if (strcmp(mode, "kills") == 0)
	// {
	// }
	if (this->Kills > GameState->kills_to_end)
	{
		GameState->ResetAllPlayersStates();
	}
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

// Source: https://forums.unrealengine.com/t/replicating-variables-in-c/79772/2 
void ACPP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_PlayerState, Kills);
	DOREPLIFETIME(ACPP_PlayerState, Deaths);
}
