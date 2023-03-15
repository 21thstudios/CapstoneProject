// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuScreen.h"

#include "OnlineSubsystem.h"
#include "GameFramework/OnlineSession.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeGameButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickResumeGameButton);
	QuitToTitleButton->OnClicked.AddDynamic(this, &UPauseMenuScreen::OnClickQuitToTitleButton);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPauseMenuScreen::OnDestroySessionComplete);
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
	if (APlayerController* PlayerController = GetOwningPlayer(); IsValid(PlayerController))
	{
		if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
			if (Session.IsValid())
			{
				// todo: Store reference to game and extract this logic there.
				//Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
				//Session->DestroySession();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Session is not valid!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController owning PauseMenu screen is not valid, therefore cannot quit to title"));
	}
}


void UPauseMenuScreen::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful);
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			Session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap", true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Destroying Session was not successful"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Session is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IOnlineSubsystem is NULL!"));
	}
	
}

