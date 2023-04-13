// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 *		This class is responsible for managing cross hairs for each specific weapon and some actions of those weapons
 */
UCLASS()
class CAPSTONEPROJECT_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CrosshairImage;

	UFUNCTION(BlueprintCallable)
	void SetCrosshair(UTexture2D* Crosshair);
	
};
