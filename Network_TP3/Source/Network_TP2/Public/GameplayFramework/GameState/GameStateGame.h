// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateGame.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API AGameStateGame : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void InvertShowDebug();
	bool GetShowDebug() const;

protected:
	UPROPERTY(Replicated)
	bool ShowDebug = true;
};
