// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SimpleNetworkSubsystem.h"

#include "Online.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

#include "Network_TP2/LogNetwork.h"

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

void USimpleNetworkSubsystem::CreateSession(const UObject* WorldContextObject, FName Name)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
		return;
	if (!IsStandalone(World))
		return;

	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;
	for (auto LocalPlayerIterator = GEngine->GetLocalPlayerIterator(World); LocalPlayerIterator; ++LocalPlayerIterator)
	{
		if (LocalPlayer == nullptr)
			LocalPlayer = *LocalPlayerIterator;
		else
		{
			return;
		}
	}
	if (LocalPlayer == nullptr)
		return;


	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(World);
	if (OnlineSubsystem == nullptr)
		return;

	const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
	if (OnlineSession == nullptr)
		return;

	FOnlineSessionSettings Settings;
	
	Settings.bIsDedicated = false;
	Settings.bIsLANMatch = false;
	Settings.NumPublicConnections = 4;
	
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowJoinViaPresence = true;
	Settings.bUseLobbiesIfAvailable = true;
	Settings.bAllowInvites = true;

	Settings.bUsesPresence = true;
	Settings.bShouldAdvertise = true;

	Settings.Set(FName("MatchType"), FString("UQAC"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	OnlineSession->OnCreateSessionCompleteDelegates.AddUObject(this, &USimpleNetworkSubsystem::OnSessionCreated);

	if (!OnlineSession->CreateSession(LocalPlayer->GetConnectionIdentifier(), Name, Settings))
	{
		OnSessionCreated(Name, false);
	}
}



void USimpleNetworkSubsystem::JoinSession(const UObject* WorldContextObject, FName Name)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
		return;
	if (!IsStandalone(World))
		return;

	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;
	for (auto LocalPlayerIterator = GEngine->GetLocalPlayerIterator(World); LocalPlayerIterator; ++LocalPlayerIterator)
	{
		if (LocalPlayer == nullptr)
			LocalPlayer = *LocalPlayerIterator;
		else
		{
			return;
		}
	}
	if (LocalPlayer == nullptr)
		return;
	
	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(World);
	if (OnlineSubsystem == nullptr)
		return;

	const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
	if (OnlineSession == nullptr)
		return;

	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	OnFindSessionsCompleteDelegate.BindUObject(this, &USimpleNetworkSubsystem::OnSessionSearchComplete);
	OnlineSession->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	
	FOnlineSessionSearch SearchSettings;
	SearchSettings.QuerySettings.Set(FName("MatchType"), FString("UQAC"), EOnlineComparisonOp::Equals);
	
	FindSessionContinuation.Emplace(Name, SearchSettings, World, LocalPlayer);
	OnlineSession->FindSessions(LocalPlayer->GetConnectionIdentifier(), MakeShared<FOnlineSessionSearch>(SearchSettings));
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

void USimpleNetworkSubsystem::OnSessionSearchComplete(bool Success)
{
	if (!Success)
		return;

	if (!FindSessionContinuation.IsSet())
		return;
	const FFindSessionContinuation Continue = FindSessionContinuation.GetValue();

	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(Continue.World);
	if (OnlineSubsystem == nullptr)
		return;

	const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface();
	if (OnlineSession == nullptr)
		return;
	
	FOnlineSessionSearchResult SearchResult;
	
	OnlineSession->OnJoinSessionCompleteDelegates.AddUObject(this, &USimpleNetworkSubsystem::OnJoinSessionComplete);
	OnlineSession->JoinSession(Continue.LocalPlayer->GetConnectionIdentifier(), Continue.SessionName,
		Continue.SearchSettings.SearchResults[0]);
}

void USimpleNetworkSubsystem::OnSessionCreated(const FName Name, const bool Success) const
{
	if (Success)
		SessionCreationSuccessDelegate.Broadcast(Name);
	else
		SessionCreationFailureDelegate.Broadcast(Name);
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
