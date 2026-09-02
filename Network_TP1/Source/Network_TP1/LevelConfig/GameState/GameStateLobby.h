// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartTimerSignature, double, TimerEnd);

/**
 * 
 */
UCLASS()
class NETWORK_TP1_API AGameStateLobby : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable)
	FOnStartTimerSignature OnStartTimerDelegate;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_TimerEnd")
	double TimerEnd = 0.0;

private:
	UFUNCTION()
	void OnRep_TimerEnd() const;
};
