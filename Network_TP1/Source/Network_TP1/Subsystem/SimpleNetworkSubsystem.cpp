// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleNetworkSubsystem.h"

void USimpleNetworkSubsystem::HostLevel(const UObject* WorldContextObject,
	const TSoftObjectPtr<UWorld> Level, const FString Options)
{
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	USimpleNetworkSubsystem::HostLevelByName(WorldContextObject, LevelName, true, Options);
}

void USimpleNetworkSubsystem::HostLevelByName(const UObject* WorldContextObject, const FName LevelName, const bool bAbsolute,
	const FString Options)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
		return;
	if (!IsStandalone(World))
		return;

	const ETravelType TravelType = (bAbsolute ? TRAVEL_Absolute : TRAVEL_Relative);
	FWorldContext &WorldContext = GEngine->GetWorldContextFromWorldChecked(World);
	
	FString Cmd = LevelName.ToString();
	Cmd += FString(TEXT("?Listen"));
	if (Options.Len() > 0)
	{
		Cmd += FString(TEXT("?")) + Options;
	}
	
	FURL TestURL(&WorldContext.LastURL, *Cmd, TravelType);
	if (TestURL.IsLocalInternal())
	{
		// make sure the file exists if we are opening a local file
		if (!GEngine->MakeSureMapNameIsValid(TestURL.Map))
		{
			UE_LOG(LogLevel, Warning, TEXT("WARNING: The map '%s' does not exist."), *TestURL.Map);
		}
	}

	GEngine->SetClientTravel(World, *Cmd, TravelType);
}

void USimpleNetworkSubsystem::JoinLevel(const UObject* WorldContextObject, const FString Address, const FString Options)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
		return;
	if (!IsStandalone(World))
		return;

	FString Cmd = Address;
	if (Options.Len() > 0)
	{
		Cmd += FString(TEXT("?")) + Options;
	}
	
	GEngine->SetClientTravel(World, *Cmd, TRAVEL_Absolute);
}

void USimpleNetworkSubsystem::ChangeLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level,
	bool bShouldSkipGameNotify)
{
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	ChangeLevelByName(WorldContextObject, LevelName, true, bShouldSkipGameNotify);
}

void USimpleNetworkSubsystem::ChangeLevelByName(const UObject* WorldContextObject, const FName LevelName, const bool bAbsolute,
	const bool bShouldSkipGameNotify)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsServer(World))
		return;
	
	World->ServerTravel(LevelName.ToString(), bAbsolute, bShouldSkipGameNotify);
}

bool USimpleNetworkSubsystem::IsStandalone(const UWorld* World)
{
	const ENetMode NetMode = World->GetNetMode();
	return NetMode == NM_Standalone;
}

bool USimpleNetworkSubsystem::IsServer(const UWorld* World)
{
	const ENetMode NetMode = World->GetNetMode();
	return NetMode == NM_ListenServer || NetMode == NM_DedicatedServer;
}
