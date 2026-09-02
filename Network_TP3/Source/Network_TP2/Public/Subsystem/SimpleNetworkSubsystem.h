// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SimpleNetworkSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionCreationSuccessDelegate, FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionCreationFailureDelegate, FName, Name);


struct FFindSessionContinuation
{
	FName SessionName;
	FOnlineSessionSearch SearchSettings;
	UWorld* World;
	TObjectPtr<ULocalPlayer> LocalPlayer;
};


/**
 * 
 */
UCLASS()
class NETWORK_TP2_API USimpleNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", AdvancedDisplay = "2",
		DisplayName = "Host Level (by Object Reference)"))
	static void HostLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level,
		FString Options = FString(TEXT("")));

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", AdvancedDisplay = "2",
		DisplayName = "Host Level (by Name)"))
	static void HostLevelByName(const UObject* WorldContextObject, FName LevelName, bool bAbsolute = true,
		FString Options = FString(TEXT("")));

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	void CreateSession(const UObject* WorldContextObject, FName Name);
	FSessionCreationSuccessDelegate SessionCreationSuccessDelegate;
	FSessionCreationFailureDelegate SessionCreationFailureDelegate;

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	void JoinSession(const UObject* WorldContextObject, FName Name);
	

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", DisplayName = "Join Server"))
	static void JoinLevel(const UObject* WorldContextObject, FString Address, FString Options = FString(TEXT("")));
	
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject",
		DisplayName = "Change level (by Object Reference)"))
	static void ChangeLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level,
		bool bShouldSkipGameNotify = false);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", DisplayName = "Change level (by Name)"))
	static void ChangeLevelByName(const UObject* WorldContextObject, FName LevelName, bool bAbsolute = true,
		bool bShouldSkipGameNotify = false);

private:
	TOptional<FFindSessionContinuation> FindSessionContinuation;
	
	void OnSessionSearchComplete(bool Success);
	void OnJoinSessionComplete(FName, EOnJoinSessionCompleteResult::Type);
	void OnSessionCreated(FName Name, bool Success) const;
	
	static bool IsStandalone(const UWorld* World);
	static bool IsServer(const UWorld* World);
};

