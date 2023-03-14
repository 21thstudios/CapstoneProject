#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "SessionListing.generated.h"

USTRUCT(BlueprintType)
struct FSessionListingInfo
{
	GENERATED_BODY()

	FSessionListingInfo()
	{
		SessionName = FName(TEXT("Missing Session Name"));
		SessionResult = static_cast<FOnlineSessionSearchResult*>(malloc(sizeof(FOnlineSessionSearchResult)));
	}

	FSessionListingInfo(FName SessionName, FOnlineSessionSearchResult* SessionResult)
	{
		this->SessionName = SessionName;
		this->SessionResult = SessionResult;
	}
	
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

	void SetSessionListingInfo(FSessionListingInfo SessionListingInfo);

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
