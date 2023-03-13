// Fill out your copyright notice in the Description page of Project Settings.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "PlayerStats.h"
#include "../CapstoneProject.h"

UPlayerStats::UPlayerStats()
{
  this->PlayerKills = 0;
  this->PlayerDeaths = 0;
}

UPlayerStats::~UPlayerStats()
{
}

void UPlayerStats::AddKill()
{
  this->PlayerKills++;
}

void UPlayerStats::AddDeath()
{
  this->PlayerDeaths++;
}

void UPlayerStats::HandleDeath(UPlayerStats* Actor)
{
  this->AddDeath();
  Actor->AddKill();
  auto PlayerNamea = Actor->GetPlayerName().ToString();
  D("Player " + PlayerNamea + " killed " + this->GetPlayerName().ToString() + "!");
}

void UPlayerStats::ClearStats()
{
    this->PlayerKills = 0;
    this->PlayerStats = 0;
}

void UPlayerStats::SetPlayerKills(int Kills)
{
  this->PlayerKills = Kills;
}

void UPlayerStats::SetPlayerDeaths(int Deaths)
{
  this->PlayerDeaths = Deaths;
}

int UPlayerStats::GetPlayerKills()
{
  return this->PlayerKills;
}

int UPlayerStats::GetPlayerDeaths()
{
  return this->PlayerDeaths;
}

FName UPlayerStats::GetPlayerName()
{
  return this->PlayerName;
}

void UPlayerStats::SetPlayerName(FName Name)
{
  this->PlayerName = Name;
}