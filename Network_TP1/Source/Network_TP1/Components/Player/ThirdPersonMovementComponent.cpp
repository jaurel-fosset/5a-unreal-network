// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonMovementComponent.h"

#include "InputActionValue.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UThirdPersonMovementComponent::UThirdPersonMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void UThirdPersonMovementComponent::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		Cast<APawn>(GetOwner())->AddControllerYawInput(Yaw);
		Cast<APawn>(GetOwner())->AddControllerPitchInput(Pitch);
	}
}

void UThirdPersonMovementComponent::DoJumpStart()
{
	Cast<ACharacter>(GetController())->Jump();
}

void UThirdPersonMovementComponent::DoJumpEnd()
{
	// signal the character to stop jumping
	Cast<ACharacter>(GetController())->StopJumping();
}

// Called when the game starts
void UThirdPersonMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(Cast<ACharacter>(GetOwner()) != nullptr, TEXT("This components needs to be attached to a Character"));
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
	return Cast<APawn>(GetOwner())->GetController();
}

void UThirdPersonMovementComponent::AddMovementInput(const FVector& WorldDirection, const float ScaleValue, const bool bForce) const
{
	return Cast<APawn>(GetOwner())->AddMovementInput(WorldDirection, ScaleValue, bForce);
}
