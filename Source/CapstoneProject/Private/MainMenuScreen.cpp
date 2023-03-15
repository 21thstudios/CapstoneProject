// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuScreen.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "SessionGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

const FName DEFAULT_SERVER_NAME = "Unnamed Server";

void UMainMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnStartOnlineGameComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnDestroySessionComplete);
	CreateGameButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickCreateGameButton);
	MultiplayerButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickMultiplayerButton);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickQuitButton);
	SessionNameEditableTextBox->OnTextChanged.AddDynamic(this, &UMainMenuScreen::OnSessionNameTextChanged);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void UMainMenuScreen::NativeDestruct()
{
	Super::NativeDestruct();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void UMainMenuScreen::OnClickCreateGameButton()
{
	USessionGameInstance SessionGameInstance = dynamic_cast<USessionGameInstance>(GetGameInstance());
	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//const TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
	const FName ServerName = SessionNameEditableTextBox ? FName(SessionNameEditableTextBox->GetText().ToString()) : DEFAULT_SERVER_NAME;
	bool bIsLan = LanCheckBox ? LanCheckBox->IsChecked() : true;
	const int32 MaxPlayers = 69;
	SessionGameInstance.StartOnlineGame(ServerName, bIsLan, true, MaxPlayers);
	//HostSession(UniqueNetId, SessionName, bIsLan, true, MaxPlayers);
}

void UMainMenuScreen::OnClickMultiplayerButton()
{
	if (MultiplayerButton)
	{
		const ESlateVisibility FlipVisibility =
			SessionList->GetVisibility() == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		SessionList->SetVisibility(FlipVisibility);
	}
}

void UMainMenuScreen::OnClickQuitButton()
{
	if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (SessionSettings.IsValid())
		{
			Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Session->DestroySession(SessionNameEditableTextBox ? FName(SessionNameEditableTextBox->GetText().ToString()): DEFAULT_SERVER_NAME);
		} 
	}
	// If we failed to destroy the session, just exit the game.
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), Player, EQuitPreference::Quit, true);
}


void UMainMenuScreen::SetHostOnLan(bool bHostOnLan)
{
	if(LanCheckBox)
	{
		LanCheckBox->SetIsChecked(bHostOnLan);
	}
}

void UMainMenuScreen::OnSessionNameTextChanged(const FText& Text)
{
	if (Text.ToString().Len() > 32)
	{
		FText clippedText = FText::FromString(SessionNameEditableTextBox->GetText().ToString().LeftChop(1));
		SessionNameEditableTextBox->SetText(clippedText);
	}
}

bool UMainMenuScreen::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN,
                                  bool bIsPresence, int32 MaxNumPlayers)
{
	if (IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 5;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = true;

			SessionSettings->Set(SETTING_MAPNAME, FString("FirstPersonMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Create session delegate triggered upon completion
			return Session->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}
	return false;
}

void UMainMenuScreen::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			// Clear the SessionComplete delegate handle because we've finished the call.
			Session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				
				// OnStartSessionComplete delegate triggered upon completion
				Session->StartSession(SessionName);
			}
		}
	}
}

void UMainMenuScreen::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (SessionSettings.IsValid())
		{
			// Clear the SessionsStart delegate handle because we've finished the call
			Session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}

		if (bWasSuccessful)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "FirstPersonMap", true, "listen");
		}
	}
}

void UMainMenuScreen::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (SessionSettings.IsValid())
		{
			// Clear the SessionsDestroy delegate handle because we've finished the call
			Session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		}
	}
}
