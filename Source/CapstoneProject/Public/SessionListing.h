#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionListing.generated.h"


UCLASS(Abstract)
class CAPSTONEPROJECT_API USessionListing : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetServerName(FText ServerName);

	void SetPlayerCount(int32 CurrentPlayers, int32 MaxPlayers);

	void SetPingMs(int32 PingInMs);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ServerNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PingInMsTextBlock;
};
