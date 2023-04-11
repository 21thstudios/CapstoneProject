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
	
	void ClearSessionListings();
	void SetUsePresence(bool bUsePresence);
	void SetSearchLobbies(bool bSearchLobbies);

	USessionListing* USessionList::CreateAndInsertSessionListingWidget(FOnlineSessionSearchResult& SearchResult, FName SessionName);

	UFUNCTION()
	void AddSessionListing(USessionListing* SessionListing);
	
	UFUNCTION()
	void OnClickRefreshButton();

	UPROPERTY(EditAnywhere, Category="Class Types")
	TSubclassOf<UUserWidget> WidgetSessionListingClass;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LANCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* PresenceCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SearchLobbiesCheckBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SessionListingsScrollBox;
};
