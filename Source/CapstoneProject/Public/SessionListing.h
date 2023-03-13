#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "SessionListing.generated.h"

USTRUCT(BlueprintType)
struct FSessionListingInfo
{
	GENERATED_BODY()
	FName SessionName;
	FOnlineSessionSearchResult* SessionResult;
};

UCLASS(Abstract)
class CAPSTONEPROJECT_API USessionListing : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	void SetServerName(FText ServerName) const;

	void SetPlayerCount(int32 CurrentPlayers, int32 MaxPlayers) const;

	void SetPingMs(int32 PingInMs) const;

	void OnClickJoinSessionButton();

	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	void SetSessionName(FName SessionName);

	void SetOnlineSessionSearchResult(FOnlineSessionSearchResult* SearchResult);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ServerNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerCountTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PingInMsTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* JoinSessionButton;
	
	FDelegateHandle JoinSessionDelegateHandle;

	UPROPERTY(BlueprintReadOnly)
	FSessionListingInfo SessionListingInfo;
};
