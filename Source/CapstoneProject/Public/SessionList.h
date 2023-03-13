#pragma once

#include "CoreMinimal.h"
#include "SessionListing.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "SessionList.generated.h"



UCLASS()
class CAPSTONEPROJECT_API USessionList : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
public:
	void SetLAN(bool bLAN);

	UFUNCTION()
	void OnClickRefreshButton();

	void OnFindSessionsComplete(bool bWasSuccessful);

	void ClearSessionListings();

	void AddSessionListing(USessionListing* SessionListing);
	
	void BindOnClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LANCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SessionListingsScrollBox;
	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
