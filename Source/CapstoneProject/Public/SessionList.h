#pragma once

#include "CoreMinimal.h"
#include "SessionListing.h"
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
	USessionList();
	
	void SetLAN(bool bLAN);

	void OnClickRefreshButton();

	void OnFindSessionsComplete(bool bWasSuccessful);

	void ClearSessionListings();

	void AddSessionListing(USessionListing* SessionListing);

	

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
