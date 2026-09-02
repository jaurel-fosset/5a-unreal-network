// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Components/PingComponent.h"
#include "PlayerControllerLobby.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API APlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerLobby();

	TObjectPtr<UPingComponent> GetPingComponent() const;
	
protected:
	virtual void OnRep_PlayerState() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ping")
	UPingComponent* PingComponent;

	virtual void SetupInputComponent() override;

private:
	bool SyncedWithPlayerState = false;
};
