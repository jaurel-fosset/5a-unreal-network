// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/PlayerController/PlayerControllerLobby.h"

#include "EnhancedInputSubsystems.h"
#include "Components/PingComponent.h"
#include "GameplayFramework/PlayerState/PlayerStateLobby.h"
#include "Network_TP2/LogNetwork.h"


APlayerControllerLobby::APlayerControllerLobby()
{
	bReplicates = true;
	PingComponent = CreateDefaultSubobject<UPingComponent>(TEXT("PingComponent"));
}

TObjectPtr<UPingComponent> APlayerControllerLobby::GetPingComponent() const
{
	return PingComponent;
}

void APlayerControllerLobby::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (HasAuthority() || SyncedWithPlayerState)
		return;
	
	GetPingComponent()->Server_SyncToServerTime();

	SyncedWithPlayerState = true;
}

void APlayerControllerLobby::SetupInputComponent()
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
