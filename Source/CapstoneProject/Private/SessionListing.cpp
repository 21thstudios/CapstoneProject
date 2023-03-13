// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionListing.h"
#include <iostream>
#include <string>
#include "Components/TextBlock.h"

void USessionListing::SetServerName(FText ServerName)
{
    //std::cout << "Server name: " << ServerName << "\n";
    if (ServerNameTextBlock)
    {
        ServerNameTextBlock->SetText(ServerName);
    }
}

void USessionListing::SetPlayerCount(int CurrentPlayers, int MaxPlayers)
{
    std::cout << "Player count: " << CurrentPlayers << "/" << MaxPlayers << "\n";
}

void USessionListing::SetPingMs(int PingInMs)
{
    std::cout << "Ping in ms: " << PingInMs << "\n";
}
