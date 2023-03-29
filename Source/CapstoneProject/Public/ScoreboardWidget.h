// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "ScoreboardEntryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "ScoreboardWidget.generated.h"

/**
 * Base user widget for scoreboards. Provided a list of PlayerState and visualizes the information within it to the
 * end user.
 */
UCLASS()
class CAPSTONEPROJECT_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SetMapName(FText MapName) const;
	
	UFUNCTION(BlueprintCallable)
	void SetServerName(FText ServerName) const;
	
	void AddEntry(FUniqueNetId* UniqueNetId, FText DisplayName, int32 Ping, int32 NumKills, int32 NumDeaths);

	void InsertEntry(UScoreboardEntryWidget* ScoreboardEntryWidget);

	UFUNCTION(BlueprintCallable)
	void UpdateEntries();
	
	UFUNCTION(BlueprintCallable)
	void ClearEntries();

	UFUNCTION(BlueprintCallable)
	void ToggleViewport();
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScoreboardEntryWidget* ScoreboardEntryWidgetHeading;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* ScoreboardEntryScrollBox;

	UPROPERTY(EditAnywhere, Category="Class Types")
	TSubclassOf<UUserWidget> ScoreboardEntryBlueprintClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ServerNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MapNameTextBlock;
};
