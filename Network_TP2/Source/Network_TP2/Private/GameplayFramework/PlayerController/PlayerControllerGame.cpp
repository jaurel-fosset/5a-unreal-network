// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/PlayerController/PlayerControllerGame.h"

#include "EnhancedInputSubsystems.h"
#include "Components/PingComponent.h"


APlayerControllerGame::APlayerControllerGame()
{
	bReplicates = true;
	PingComponent = CreateDefaultSubobject<UPingComponent>(TEXT("PingComponent"));
}

TObjectPtr<UPingComponent> APlayerControllerGame::GetPingComponent() const
{
	return PingComponent;
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
