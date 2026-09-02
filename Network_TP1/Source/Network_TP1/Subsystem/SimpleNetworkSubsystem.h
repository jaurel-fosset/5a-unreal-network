// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SimpleNetworkSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP1_API USimpleNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", AdvancedDisplay = "2", DisplayName = "Host Level (by Object Reference)"))
	static void HostLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level,
		FString Options = FString(TEXT("")));

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", AdvancedDisplay = "2", DisplayName = "Host Level (by Name)"))
	static void HostLevelByName(const UObject* WorldContextObject, FName LevelName, bool bAbsolute = true, FString Options = FString(TEXT("")));
	

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", DisplayName = "Join Server"))
	static void JoinLevel(const UObject* WorldContextObject, FString Address, FString Options = FString(TEXT("")));

	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", DisplayName = "Change level (by Object Reference)"))
	static void ChangeLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, bool bShouldSkipGameNotify = false);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", DisplayName = "Change level (by Name)"))
	static void ChangeLevelByName(const UObject* WorldContextObject, FName LevelName, bool bAbsolute = true, bool bShouldSkipGameNotify = false);

private:
	static bool IsStandalone(const UWorld* World);
	static bool IsServer(const UWorld* World);
};
