// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "CPP_PlayerState.h"
#include "CPP_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include <string>

#include "../../../../../../../../Program Files/Epic Games/UE_5.1/Engine/Platforms/Hololens/Source/Runtime/Core/Public/Microsoft/MinWindowsPrivate.h"
#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(x).c_str());}
#define DFstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

ACPP_PlayerState::ACPP_PlayerState()
{
	this->Kills = 0;
	this->Deaths = 0;
	// Replace with proper way to get player name once available 
	this->Name = std::to_string(this->GetUniqueID()).c_str();
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
	FString KillerName = this->Name.ToString();
	FString DeathName = OtherPlayer->Name.ToString();
	FString DisplayMessage = KillerName + " killed " + DeathName + "!";
	DFstr(DisplayMessage);
	ACPP_GameState* GameState = GetWorld()->GetGameState<ACPP_GameState>();
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

void ACPP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_PlayerState, Kills);
	DOREPLIFETIME(ACPP_PlayerState, Deaths);
	DOREPLIFETIME(ACPP_PlayerState, Name);
}
