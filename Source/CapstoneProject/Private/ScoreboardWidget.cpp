// Copyright (c) 21thStudios contributors. All rights reserved. Licensed under the MIT license.


#include "ScoreboardWidget.h"

#include <chrono>

#include "CPP_GameState.h"
#include "CPP_PlayerState.h"
#include "SessionGameInstance.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OnRefreshScoreboard();

	GetWorld()->GetTimerManager().SetTimer(RefreshScoreboardTimerHandle, this, &UScoreboardWidget::OnRefreshScoreboard, ScoreboardDelayInSecondsPerRefresh, true);
}

void UScoreboardWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UScoreboardWidget::SetRemainingTimeInSeconds(int32 RemainingTimeSeconds)
{
	if (RemainingTimeSecondsTextBlock)
	{
		int32 RemainingTimeMinutes = RemainingTimeSeconds / 60;
		int32 LeftoverSeconds = RemainingTimeSeconds % 60;
		FString RemainingTimeMinutesString = RemainingTimeMinutes > 0 ? FString::FromInt(RemainingTimeMinutes) : "";
		FString RemainingTimeSecondsString = RemainingTimeMinutes > 0 && LeftoverSeconds < 10 ? FString("0").Append(FString::FromInt(LeftoverSeconds)) : FString::FromInt(LeftoverSeconds);
		
		FFormatNamedArguments Args;
		Args.Add("MinutesRemaining", FText::FromString(RemainingTimeMinutesString));
		Args.Add("MinutesRemainingDelimiter", RemainingTimeMinutes > 0 ? FText::FromString(":") : FText::FromString(""));
		Args.Add("SecondsRemaining", FText::FromString(RemainingTimeSecondsString));
		FText FormattedText = FText::Format(
	NSLOCTEXT("Scoreboard", "RemainingTimeFormat", "{MinutesRemaining}{MinutesRemainingDelimiter}{SecondsRemaining}"), Args);
		RemainingTimeSecondsTextBlock->SetText(FormattedText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add entry due to invalid RemainingTimeSecondsTextBlock!"));
	}
}

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
		ACPP_GameState* GameState = static_cast<ACPP_GameState*>(UGameplayStatics::GetGameState(GetWorld()));
		SetRemainingTimeInSeconds(FMath::Max(GameState->GetSecondsRemainingOfGame(), 0.f));
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
