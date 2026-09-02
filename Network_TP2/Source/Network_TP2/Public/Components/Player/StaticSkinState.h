// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StaticSkinState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UStaticSkinState : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaticSkinState();

	FGameplayTag GetSkin() const;
	UFUNCTION(Server, Reliable)
	void Server_SetupSkin(FGameplayTag NewSkin);
	
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category="Skin")
	FGameplayTag Skin;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	bool IsSkinSetup = false;

	void SetupSkin(FGameplayTag NewSkin);
};
