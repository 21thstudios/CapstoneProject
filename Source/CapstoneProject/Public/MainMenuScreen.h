// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SessionList.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "MainMenuScreen.generated.h"

/**
 * Main menu widget screen players see when joining the game.
 */
UCLASS()
class CAPSTONEPROJECT_API UMainMenuScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void SetHostOnLan(bool bHostOnLAN);

	UFUNCTION()
	void OnSessionNameTextChanged(const FText& Text);
	
	UFUNCTION(BlueprintCallable)
	void OnClickCreateGameButton();

	UFUNCTION(BlueprintCallable)
	void OnClickMultiplayerButton();
	
	UFUNCTION(BlueprintCallable)
	void OnClickQuitButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* ServerNameEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USessionList* SessionList;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreateGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MultiplayerButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitGameButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LanCheckBox;
};