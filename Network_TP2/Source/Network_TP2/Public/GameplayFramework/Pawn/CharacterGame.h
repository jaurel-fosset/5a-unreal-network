// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ServerRewind.h"
#include "Components/Player/ShootComponent.h"
#include "Components/Player/StaticSkinComponent.h"
#include "Components/Player/ThirdPersonMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayFramework/PlayerController/PlayerControllerGame.h"
#include "GameplayFramework/PlayerState/PlayerStateGame.h"
#include "CharacterGame.generated.h"

UCLASS()
class NETWORK_TP2_API ACharacterGame : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterGame();
	
	void SetupServerRewindCapsule(const double Timestamp) const;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ShootWithRewind(const double Timestamp);
	UFUNCTION(BlueprintCallable)
	void Shoot();


protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Restart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	TObjectPtr<APlayerStateGame> GetPlayerStateGame() const;
	TObjectPtr<APlayerControllerGame> GetPlayerControllerGame() const;

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Death"))
	void OnDeathBlueprintEvent();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Death Prediction"))
	void OnDeathPredictionBlueprintEvent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Respawn")
	TObjectPtr<APlayerStart> PlayerStart;
	
	
	UPROPERTY(EditAnywhere, Category="Component")
	TObjectPtr<UThirdPersonMovementComponent> ThirdPersonMovementComponent;

	UPROPERTY(EditAnywhere, Category="Component")
	TObjectPtr<UStaticSkinComponent> SkinComponent;

	UPROPERTY(EditAnywhere, Category="Component")
	TObjectPtr<UShootComponent> ShootComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Rewind")
	TObjectPtr<UCapsuleComponent> ServerRewindCapsule;

	UPROPERTY(EditAnywhere, Category = "Server Rewind")
	TEnumAsByte<ECollisionChannel> ServerRewindCollisionChannel;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

private:
	UPROPERTY()
	TObjectPtr<UServerRewind> ServerRewind;
};
