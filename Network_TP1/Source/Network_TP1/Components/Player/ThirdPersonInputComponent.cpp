// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonInputComponent.h"

#include "EnhancedInputComponent.h"
#include "LogNetworkTP.h"


// Sets default values for this component's properties
UThirdPersonInputComponent::UThirdPersonInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UThirdPersonInputComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(MovementComponent != nullptr, TEXT("This component should be linked to a ThirdPersonMovementComponent"));
}

void UThirdPersonInputComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, MovementComponent, &UThirdPersonMovementComponent::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, MovementComponent, &UThirdPersonMovementComponent::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, MovementComponent, &UThirdPersonMovementComponent::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, MovementComponent, &UThirdPersonMovementComponent::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, MovementComponent, &UThirdPersonMovementComponent::Look);
	}
	else
	{
		UE_LOG(LogNetworkTP, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
