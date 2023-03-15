#include "MainMenuScreen.h"

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
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMainMenuScreen::OnDestroySessionComplete);
	CreateGameButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickCreateGameButton);
	MultiplayerButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickMultiplayerButton);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuScreen::OnClickQuitButton);
	ServerNameEditableTextBox->OnTextChanged.AddDynamic(this, &UMainMenuScreen::OnSessionNameTextChanged);

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
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	const FName ServerName = ServerNameEditableTextBox ? FName(ServerNameEditableTextBox->GetText().ToString()) : DEFAULT_SERVER_NAME;
	bool bIsLan = LanCheckBox ? LanCheckBox->IsChecked() : true;
	const int32 MaxPlayers = 69;
	SessionGameInstance->StartOnlineGame(ServerName, bIsLan, true, MaxPlayers);
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
			Session->DestroySession(ServerNameEditableTextBox ? FName(ServerNameEditableTextBox->GetText().ToString()): DEFAULT_SERVER_NAME);
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
		FText clippedText = FText::FromString(ServerNameEditableTextBox->GetText().ToString().LeftChop(1));
		ServerNameEditableTextBox->SetText(clippedText);
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
