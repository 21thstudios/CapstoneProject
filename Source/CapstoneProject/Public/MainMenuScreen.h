// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/** Fires upon session create request completion */
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/** Fires upon session start request completion */
	virtual void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	void OnClickCreateGameButton();
	
	void SetHostOnLan(bool bHostOnLAN);
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreateGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LanCheckBox;
	
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
};