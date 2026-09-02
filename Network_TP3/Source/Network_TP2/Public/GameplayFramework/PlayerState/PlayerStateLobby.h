// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PingComponent.h"
#include "Components/Player/DynamicSkinState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateLobby.generated.h"


/**
 * 
 */
UCLASS()
class NETWORK_TP2_API APlayerStateLobby : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerStateLobby();
	TObjectPtr<UDynamicSkinState> GetComponentSkin() const;

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player State")
	UDynamicSkinState* SkinState;
};
