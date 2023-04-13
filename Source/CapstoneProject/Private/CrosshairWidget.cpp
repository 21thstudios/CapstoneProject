// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "CrosshairWidget.h"

#include "Components/Image.h"

void UCrosshairWidget::SetCrosshair(UTexture2D* Crosshair)
{
	if (CrosshairImage)
	{
		CrosshairImage->SetBrushFromTexture(Crosshair);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set name of scoreboard entry due to invalid PlayerDisplayNameTextBlock!"));
	}
}
