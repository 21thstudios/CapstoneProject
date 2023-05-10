// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.

#pragma once

#include "CoreMinimal.h"
#include "ScoreboardEntryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "ScoreboardWidget.generated.h"

/**
 * Base user widget for scoreboards. Rebuilds once per second while it is open.
 */
UCLASS()
class CAPSTONEPROJECT_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetRemainingTimeInSeconds(int32 RemainingTimeSeconds);
	
	void AddEntry(FScoreboardEntryData* ScoreboardEntryData);
	
	void InsertEntry(UScoreboardEntryWidget* ScoreboardEntryWidget);
	
	UFUNCTION(BlueprintCallable)
	void ClearEntries();

	UFUNCTION(BlueprintCallable)
	void ToggleViewport(bool ShouldAddToViewport);

	UFUNCTION(BlueprintCallable)
	void OnRefreshScoreboard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle RefreshScoreboardTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ScoreboardDelayInSecondsPerRefresh = 1.f;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScoreboardEntryWidget* ScoreboardEntryWidgetHeading;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* ScoreboardEntryScrollBox;

	UPROPERTY(EditAnywhere, Category="Class Types")
	TSubclassOf<UUserWidget> ScoreboardEntryBlueprintClass;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RemainingTimeSecondsTextBlock;
};
