// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/GameState/GameStateGame.h"

#include "Net/UnrealNetwork.h"

void AGameStateGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameStateGame, ShowDebug);
}

void AGameStateGame::InvertShowDebug()
{
	ShowDebug = !ShowDebug;
}

bool AGameStateGame::GetShowDebug() const
{
	return ShowDebug;
}
