#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "PauseMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UPauseMenuScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void OnClickResumeGameButton();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void OnClickQuitToTitleButton();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitToTitleButton;

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};
