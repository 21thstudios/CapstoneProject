// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

// Source: https://forums.unrealengine.com/t/print-to-screen-using-c/357351/3 
#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

#include "CPP_PlayerState.h"

ACPP_PlayerState::ACPP_PlayerState()
{
	this->Kills = 0;
	this->Deaths = 0;
	this->Name = "a";
	D("scree");
}

