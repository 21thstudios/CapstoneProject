// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#include "CPP_GameState.h"
#include "CPP_PlayerState.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"

// Debug print, with quotations. E.g., `D("Hello World");` 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Debug print for `int`, `string`, and `FString`. 
#define Dnum(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, std::to_string(x).c_str());}
#define Dstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x.ToString());}
#define DFstr(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

ACPP_GameState::ACPP_GameState()
{
	this->mode = TEXT("time");
	this->kills_to_end = 1;
	this->GameEndTimeInSeconds = 90.f;
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
		GetWorld()->ServerTravel("/Game/Maps/BloodGulch/BloodGulch?listen", true);
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

float ACPP_GameState::GetSecondsRemainingOfGame() const
{
	return GameEndTimeInSeconds - GetServerWorldTimeSeconds();
}

FText ACPP_GameState::GetFormattedTimeRemainingOfGame()
{
	int32 RemainingTimeSeconds = FMath::Max(GetSecondsRemainingOfGame(), 0.);
	int32 RemainingTimeMinutes = RemainingTimeSeconds / 60;
	int32 LeftoverSeconds = RemainingTimeSeconds % 60;
	FString RemainingTimeMinutesString = RemainingTimeMinutes > 0 ? FString::FromInt(RemainingTimeMinutes) : "";
	FString RemainingTimeSecondsString = RemainingTimeMinutes > 0 && LeftoverSeconds < 10 ? FString("0").Append(FString::FromInt(LeftoverSeconds)) : FString::FromInt(LeftoverSeconds);
		
	FFormatNamedArguments Args;
	Args.Add("MinutesRemaining", FText::FromString(RemainingTimeMinutesString));
	Args.Add("MinutesRemainingDelimiter", RemainingTimeMinutes > 0 ? FText::FromString(":") : FText::FromString(""));
	Args.Add("SecondsRemaining", FText::FromString(RemainingTimeSecondsString));
	FText FormattedText = FText::Format(
NSLOCTEXT("Scoreboard", "RemainingTimeFormat", "{MinutesRemaining}{MinutesRemainingDelimiter}{SecondsRemaining}"), Args);
	return FormattedText;
}

bool ACPP_GameState::ShouldEndGameByTime()
{
	return GetSecondsRemainingOfGame() <= 0;
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

void ACPP_GameState::HandleGameEnd() 
{
	HandleGameEndByTime();
	HandleGameEndByKills();
}

void ACPP_GameState::BeginPlay() 
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(this->GameEndTimer, this, &ACPP_GameState::HandleGameEnd, 1.0f, true);
}