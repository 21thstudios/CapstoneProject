#include "MainMenuScreen.h"

#include "SessionGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

const FName DEFAULT_SERVER_NAME = "Unnamed Server";

void UMainMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
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
	CreateGameButton->OnClicked.RemoveDynamic(this, &UMainMenuScreen::OnClickCreateGameButton);
	MultiplayerButton->OnClicked.RemoveDynamic(this, &UMainMenuScreen::OnClickMultiplayerButton);
	QuitGameButton->OnClicked.RemoveDynamic(this, &UMainMenuScreen::OnClickQuitButton);
	ServerNameEditableTextBox->OnTextChanged.RemoveDynamic(this, &UMainMenuScreen::OnSessionNameTextChanged);
}

void UMainMenuScreen::OnClickCreateGameButton()
{
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	const FName ServerName = ServerNameEditableTextBox ? FName(ServerNameEditableTextBox->GetText().ToString()) : DEFAULT_SERVER_NAME;
	bool bIsLan = LanCheckBox ? LanCheckBox->IsChecked() : true;
	bool bUsesPresence = PresenceCheckBox ? PresenceCheckBox->IsChecked() : true;
	bool bUseLobbies = false;
	const int32 MaxPlayers = 20;
	SessionGameInstance->StartOnlineGame(ServerName, bIsLan, bUsesPresence, MaxPlayers, bUseLobbies);
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
	USessionGameInstance* SessionGameInstance = dynamic_cast<USessionGameInstance*>(GetGameInstance());
	SessionGameInstance->DestroySessionAndLeaveGame();
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

void UMainMenuScreen::SetUsePresence(bool bUsePresence)
{
	if (PresenceCheckBox)
	{
		PresenceCheckBox->SetIsChecked(bUsePresence);
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
