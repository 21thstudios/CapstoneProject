// Fill out your copyright notice in the Description page of Project Settings.


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
