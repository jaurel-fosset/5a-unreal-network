// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "LogNetworkTP.h"

AGameModeMain::AGameModeMain()
{
	bUseSeamlessTravel = true;
}

APlayerController* AGameModeMain::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogNetworkTP, Display, TEXT("[GameModeMain] New connection with option : %s"), *Options);
	
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}
