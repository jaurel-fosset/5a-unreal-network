// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Player/StaticSkinState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateGame.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API APlayerStateGame : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerStateGame();
	TObjectPtr<UStaticSkinState> GetComponentSkin() const;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player State")
	UStaticSkinState* SkinState;
};
