// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Online.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionGameInstance.generated.h"

/**
 * Extension of the GameInstance responsible for Sessions-related work.
 */
UCLASS()
class CAPSTONEPROJECT_API USessionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USessionGameInstance(const FObjectInitializer& ObjectInitializer);
	
	/** Creating online sessions */
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/** Finding online sessions */
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	virtual void OnFindSessionsComplete(bool bWasSuccessful);

	/** Joining online sessions */
	virtual bool JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	/** Creating online session delegates */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/** Finding online session delegates */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/** Joining online session delegates */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
