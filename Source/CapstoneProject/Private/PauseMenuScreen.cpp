// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuScreen.h"

#include "OnlineSubsystem.h"
#include "SessionGameInstance.h"
#include "GameFramework/OnlineSession.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeGameButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickResumeGameButton);
	QuitToTitleButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickQuitToTitleButton);
}

void UPauseMenuScreen::NativeDestruct()
{
	Super::NativeConstruct();
	ResumeGameButton->OnClicked.RemoveDynamic(this, &UPauseMenuScreen::OnClickResumeGameButton);
	QuitToTitleButton->OnClicked.RemoveDynamic(this, &UPauseMenuScreen::OnClickQuitToTitleButton);
}

void UPauseMenuScreen::OnClickResumeGameButton()
{
	ToggleViewport();
}

void UPauseMenuScreen::ToggleViewport()
{
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		if (this->IsInViewport())
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
			this->RemoveFromParent();
			
		}
		else
		{
			PlayerController->FlushPressedKeys();
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
			this->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController owning PauseMenu screen is not valid, therefore cannot pause/unpause"));
	}
}


void UPauseMenuScreen::OnClickQuitToTitleButton()
{
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	SessionGameInstance->DestroySessionAndLeaveGame();
}
