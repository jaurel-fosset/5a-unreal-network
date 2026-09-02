// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/Player/DynamicSkinComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayFramework/PlayerState/PlayerStateLobby.h"
#include "Network_TP2/Public/Components/Player/ThirdPersonMovementComponent.h"
#include "CharacterLobby.generated.h"

UCLASS()
class NETWORK_TP2_API ACharacterLobby : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterLobby();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UThirdPersonMovementComponent> ThirdPersonMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDynamicSkinComponent> SkinComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

private:
	TObjectPtr<APlayerStateLobby> GetPlayerStateLobby();

	void SyncWithPlayerState();
};
