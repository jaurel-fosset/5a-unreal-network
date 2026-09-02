// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_TP2/Public/Components/Player/ThirdPersonMovementComponent.h"
#include "Network_TP2/LogNetwork.h"

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"



// Sets default values for this component's properties
UThirdPersonMovementComponent::UThirdPersonMovementComponent()
{
}

void UThirdPersonMovementComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Owner = Cast<ACharacter>(GetOwner());
	checkf(Owner, TEXT("This component can only be placed on ACharacter"));
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, Owner.Get(), &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, Owner.Get(), &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UThirdPersonMovementComponent::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &UThirdPersonMovementComponent::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UThirdPersonMovementComponent::Look);
	}
	else
	{
		UE_LOG(LogNetworkTP, Error, TEXT("'%s' Failed to find an Enhanced Input component! This component is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


// Called when the game starts
void UThirdPersonMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	checkf(Owner, TEXT("This component can only be placed on ACharacter"));
}

void UThirdPersonMovementComponent::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void UThirdPersonMovementComponent::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

TObjectPtr<AController> UThirdPersonMovementComponent::GetController() const
{
	return Owner->GetController();
}

void UThirdPersonMovementComponent::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		Owner->AddMovementInput(ForwardDirection, Forward);
		Owner->AddMovementInput(RightDirection, Right);
	}
}

void UThirdPersonMovementComponent::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		Owner->AddControllerYawInput(Yaw);
		Owner->AddControllerPitchInput(Pitch);
	}
}

void UThirdPersonMovementComponent::DoJumpStart()
{
	// signal the character to jump
	Owner->Jump();
}

void UThirdPersonMovementComponent::DoJumpEnd()
{
	// signal the character to stop jumping
	Owner->StopJumping();
}
