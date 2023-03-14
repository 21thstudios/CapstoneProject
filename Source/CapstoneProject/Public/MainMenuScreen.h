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
	
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/** Fires upon session create request completion */
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/** Fires upon session start request completion */
	virtual void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	
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
	class UEditableTextBox* SessionNameEditableTextBox;

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
	
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};