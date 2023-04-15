// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#include "CPP_GameState.h"
#include "CPP_PlayerState.h"
#include "Kismet/GameplayStatics.h"

// Debug print, with quotations. E.g., `D("Hello World");` 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Debug print for `int`, `string`, and `FString`. 
#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(x).c_str());}
#define Dstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x.ToString());}
#define DFstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

ACPP_GameState::ACPP_GameState()
{
	this->mode = TEXT("kills");
	this->kills_to_end = 3;
	this->time_to_end = 60;
	this->bReplicates = true;
}

void ACPP_GameState::ResetStateForNewGame()
{
	if (HasAuthority()) {
		int HighestKills = -1;
		ACPP_PlayerState* StateWithHighestKills = NULL;
		for (auto Player : this->PlayerArray)
		{
            ACPP_PlayerState* Stats = static_cast<ACPP_PlayerState*>(Player);
			if (Stats->Kills > HighestKills)  {
				HighestKills = Stats->Kills;
				StateWithHighestKills = Stats;
			}
		}
		FString WinnerName = StateWithHighestKills->Name.ToString();
		FString NumberOfKills = FString::FromInt(StateWithHighestKills->Kills);
		FString DisplayMessage = WinnerName + " won the game with " + NumberOfKills + " kills!";

		DFstr(DisplayMessage);
		D("Resetting all player stats.");
    this->ResetAllPlayersStates();
	}
}

void ACPP_GameState::ResetAllPlayersStates()
{
	for (auto Player : this->PlayerArray)
	{
		ACPP_PlayerState* Stats = static_cast<ACPP_PlayerState*>(Player);
		Stats->ResetKillsAndDeaths();
	}
}

bool ACPP_GameState::ShouldEndGameByTime()
{
  return this->GetTimeSinceGameStart() >= this->time_to_end;
}

bool ACPP_GameState::ShouldEndGameByKills()
{
  if (this->PlayerArray.Num() == 0) {
    return false;
  }
  for (auto Player : this->PlayerArray)
  {
    ACPP_PlayerState* Stats = static_cast<ACPP_PlayerState*>(Player);
    if (Stats->Kills >= this->kills_to_end) {
      return true;
    }
  }
  return false;
}

void ACPP_GameState::HandleGameEndByKills()
{
  if (this->HasAuthority() && this->ShouldEndGameByKills()) {
    this->ResetStateForNewGame();
  }
}

void ACPP_GameState::HandleGameEndByTime()
{
  if (this->HasAuthority() && this->ShouldEndGameByTime()) {
    this->ResetStateForNewGame();
  }
}

int ACPP_GameState::GetTimeSinceGameStart()
{
  return FDateTime::Now().ToUnixTimestamp() - this->GetServerWorldTimeSeconds();
}

void ACPP_GameState::HandleGameEnd() 
{
  if (this->mode == TEXT("time")) {
    this->HandleGameEndByTime();
  } else if (this->mode == TEXT("kills")) {
    this->HandleGameEndByKills();
  } else {
    D("Invalid game mode.");
  }
}

void ACPP_GameState::BeginPlay() 
{
	Super::BeginPlay();
	this->time_to_end = GetServerWorldTimeSeconds() + 60.f;
	GetWorldTimerManager().SetTimer(this->GameEndTimer, this, &ACPP_GameState::HandleGameEnd, 1.0f, true);
}