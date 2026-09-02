// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateGame.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP1_API APlayerStateGame : public APlayerState
{
	GENERATED_BODY()
	
public:
	APlayerStateGame();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FGameplayTag GetSkin() const;
	void SetupSkin(FGameplayTag NewSkin);
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Skin", meta=(AllowPrivateAccess))
	FGameplayTag Skin;
};
