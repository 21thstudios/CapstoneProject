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
	this->SetGameStartTimeToNow();
	this->mode = TEXT("time");
	this->kills_to_end = 3;
	this->time_to_end = 20;
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
	    this->SetGameStartTimeToNow();
	}
}

void ACPP_GameState::ResetAllPlayersStates()
{
	// Source: https://forums.unrealengine.com/t/how-to-get-the-player-controller-with-c/287177
	for (auto Player : this->PlayerArray)
	{
		// Player.ResetKillsAndDeaths();
		ACPP_PlayerState* Stats = static_cast<ACPP_PlayerState*>(Player);
		// D("Kills for current player:");
		// Dnum(Stats->Kills);
		Stats->ResetKillsAndDeaths();
	}
}

void ACPP_GameState::SetGameStartTimeToNow()
{
	this->time_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);	
}

bool ACPP_GameState::ShouldEndGameByTime()
{
  return this->GetTimeSinceGameStart() >= this->time_to_end;
}

bool ACPP_GameState::ShouldEndGameByKills()
{
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
  int now = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
    ).count();
  return now - this->StartTime();
}

void ACPP_GameState::Tick() 
{
  if (this->mode == TEXT("time")) {
    this->HandleGameEndByTime();
  } else if (this->mode == TEXT("kills")) {
    this->HandleGameEndByKills();
  } else {
    D("Invalid game mode.");
  }
}

int ACPP_GameState::StartTime() {
  return this->time_at_start.count();
}

void ACPP_GameState::BeginPlay() 
{
	Super::BeginPlay();
	// Source: https://www.tomlooman.com/unreal-engine-cpp-timers/ 
	/*
  FTimerHandle GameEndTimer;
	FTimerDelegate Delegate;
  
  if (this->mode == TEXT("time")) {
	  GetWorld()->GetTimerManager().SetTimer(GameEndTimer, this, &ACPP_GameState::ResetStateForNewGame, this->time_to_end, true);
  } else if (this->mode == TEXT("kills")) {
    GetWorld()->GetTimerManager().SetTimer(GameEndTimer, this, &ACPP_GameState::ResetStateForNewGame, 1.0f, true);
  } else {
    D("Invalid game mode.");
  }
  */
}
