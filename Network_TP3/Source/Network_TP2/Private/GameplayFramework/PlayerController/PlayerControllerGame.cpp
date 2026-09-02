// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/PlayerController/PlayerControllerGame.h"

#include "EnhancedInputSubsystems.h"
#include "Components/PingComponent.h"
#include "GameplayFramework/GameState/GameStateGame.h"


APlayerControllerGame::APlayerControllerGame()
{
	bReplicates = true;
	PingComponent = CreateDefaultSubobject<UPingComponent>(TEXT("PingComponent"));
}

TObjectPtr<UPingComponent> APlayerControllerGame::GetPingComponent() const
{
	return PingComponent;
}

void APlayerControllerGame::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (HasAuthority() || SyncedWithPlayerState)
		return;
	
	GetPingComponent()->Server_SyncToServerTime();

	SyncedWithPlayerState = true;
}

void APlayerControllerGame::InvertShowDebug_Implementation()
{
	const TObjectPtr<AGameStateGame> GameStateGame = GetWorld()->GetGameState<AGameStateGame>();
	GameStateGame->InvertShowDebug();
}

void APlayerControllerGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (const UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
