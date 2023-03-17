// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#include "CPP_GameState.h"
#include <chrono>
#include <string>

// Source for chrono use: https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
// Source for string use (make sure to include the header): https://stackoverflow.com/questions/31860405/how-to-concatenate-a-string-with-chronomilliseconds

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

ACPP_GameState::ACPP_GameState()
{
	this->SetGameStartTimeToNow();

	// std::to_string(ms.count())
	if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(this->time_at_start.count()).c_str());}
}

void ACPP_GameState::SetGameStartTimeToNow()
{
	this->time_at_start = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);	
}

void ACPP_GameState::ResetStateForNewGame()
{
	this->ResetAllPlayersStates();
	this->SetGameStartTimeToNow();
}

void ACPP_GameState::ResetAllPlayersStates()
{
	
}