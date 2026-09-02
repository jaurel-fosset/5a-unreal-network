// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/GameMode/GameModeGame.h"

#include "Network_TP2/LogNetwork.h"

AGameModeGame::AGameModeGame()
{
	bUseSeamlessTravel = true;
}

FString AGameModeGame::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	UE_LOG(LogNetworkTP, Display, TEXT("Created a Player Controller : %s"), *NewPlayerController->GetName());
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AGameModeGame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogNetworkTP, Display, TEXT("New Connection : Player Controller %s"), *NewPlayer->GetName());
}
