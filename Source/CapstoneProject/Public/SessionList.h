#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "SessionList.generated.h"



UCLASS()
class CAPSTONEPROJECT_API USessionList : public UUserWidget
{
	GENERATED_BODY()

public :
	void SetLAN(bool LAN);

	void OnClickRefreshButton();

	void ClearSessionListings();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LANCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SessionListingsScrollBox;
};
