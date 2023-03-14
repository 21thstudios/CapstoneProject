// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuScreen.h"

#include "Components/Button.h"

void UPauseMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeGameButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickResumeGameButton);
	QuitToTitleButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickQuitToTitleButton);
}

void UPauseMenuScreen::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPauseMenuScreen::OnClickResumeGameButton()
{
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		this->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController owning PauseMenu screen is not valid, therefore cannot unpause"));
	}
}

void UPauseMenuScreen::OnClickQuitToTitleButton()
{
	
}
