#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UPauseMenuScreen : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnClickResumeGameButton();

	UFUNCTION()
	void OnClickQuitToTitleButton();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitToTitleButton;
};
