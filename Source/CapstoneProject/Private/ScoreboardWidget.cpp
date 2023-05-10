// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include "SessionGameInstance.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//OnRefreshScoreboard();

	//GetWorld()->GetTimerManager().SetTimer(RefreshScoreboardTimerHandle, this, &UScoreboardWidget::OnRefreshScoreboard, ScoreboardDelayInSecondsPerRefresh, true);
}

void UScoreboardWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

/*
void UScoreboardWidget::AddEntry(FScoreboardEntryData* ScoreboardEntryData)
{
	if (ScoreboardEntryScrollBox)
	{
		const FName WidgetName = FName(ScoreboardEntryData->UniqueNetId->ToString());
		UScoreboardEntryWidget* ScoreboardEntry = CreateWidget<UScoreboardEntryWidget>(this->GetOwningPlayer(), ScoreboardEntryBlueprintClass, WidgetName);
		ScoreboardEntry->SetAll(ScoreboardEntryData);
		InsertEntry(ScoreboardEntry);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add entry due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::InsertEntry(UScoreboardEntryWidget* ScoreboardEntryWidget)
{
	if (ScoreboardEntryScrollBox)
	{
		ScoreboardEntryScrollBox->AddChild(ScoreboardEntryWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to insert entry due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::OnRefreshScoreboard()
{
	if (ScoreboardEntryScrollBox)
	{
		ClearEntries();
		TArray<TObjectPtr<APlayerState>> PlayerArray = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
		
		for (APlayerState* PS : PlayerArray)
		{
			ACPP_PlayerState* PlayerState = static_cast<ACPP_PlayerState*>(PS);
			FScoreboardEntryData ScoreboardEntryData = FScoreboardEntryData();
			ScoreboardEntryData.NumDeaths = PlayerState->Deaths;
			ScoreboardEntryData.NumKills = PlayerState->Kills;
			ScoreboardEntryData.PingInMillis = PlayerState->GetCompressedPing() * 4;
			ScoreboardEntryData.UniqueNetId = PlayerState->GetUniqueId().GetV1().Get();
			ScoreboardEntryData.SteamDisplayName = FText::FromString(PS->GetPlayerName());
			AddEntry(&ScoreboardEntryData);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to update entries due to invalid ScoreboardEntryWidget!"));
	}
}

void UScoreboardWidget::ClearEntries()
{
	if (ScoreboardEntryScrollBox)
	{
		ScoreboardEntryScrollBox->ClearChildren();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to clear entries due to invalid ScoreboardEntryWidget!"));
	}
}
*/

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
