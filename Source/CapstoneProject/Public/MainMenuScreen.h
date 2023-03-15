// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SessionList.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "MainMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UMainMenuScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void SetHostOnLan(bool bHostOnLAN);

	UFUNCTION()
	void OnSessionNameTextChanged(const FText& Text);
	
	UFUNCTION()
	void OnClickCreateGameButton();

	UFUNCTION()
	void OnClickMultiplayerButton();
	
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void OnClickQuitButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* ServerNameEditableTextBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USessionList* SessionList;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreateGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MultiplayerButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitGameButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LanCheckBox;
	
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};