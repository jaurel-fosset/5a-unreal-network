// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Components/PingComponent.h"
#include "Components/ServerRewind.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerGame.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API APlayerControllerGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerGame();
	TObjectPtr<UPingComponent> GetPingComponent() const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ping")
    UPingComponent* PingComponent;
	
	virtual void SetupInputComponent() override;
};
