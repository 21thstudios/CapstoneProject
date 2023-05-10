// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include "SessionGameInstance.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScoreboardWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UScoreboardWidget::ToggleViewport(bool ShouldAddToViewport)
{
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		if (this->IsInViewport() && !ShouldAddToViewport)
		{
			this->RemoveFromParent();
		}
		else
		{
			this->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController owning Scoreboard screen is not valid, therefore cannot pause/unpause"));
	}
}
