// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#include "CPP_GameState.h"
#include "CPP_PlayerState.h"
#include <chrono>
#include <string>
#include "Kismet/GameplayStatics.h"

// Source for chrono use: https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
// Source for string use (make sure to include the header): https://stackoverflow.com/questions/31860405/how-to-concatenate-a-string-with-chronomilliseconds

// Debug print, with quotations. E.g., `D("Hello World");` 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}
// Debug print with automatically converting an integer into a printable form. 
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

	// std::to_string(ms.count())
	// if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(this->time_at_start.count()).c_str());}
}

void ACPP_GameState::SetGameStartTimeToNow()
{
	this->time_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);	
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

void ACPP_GameState::BeginPlay() 
{
	Super::BeginPlay();
	// Source: https://www.tomlooman.com/unreal-engine-cpp-timers/ 
	FTimerHandle GameEndTimer;
	FTimerDelegate Delegate;
	GetWorld()->GetTimerManager().SetTimer(GameEndTimer, this, &ACPP_GameState::ResetStateForNewGame, this->time_to_end, true);
}
