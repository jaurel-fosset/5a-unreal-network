// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StaticSkinComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UStaticSkinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaticSkinComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetupSkin(const FGameplayTag NewSkin);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Skin", meta=(DisplayName="Skin Changed"))
	void SetupSkinBlueprintEvent(const FGameplayTag NewSkin);
};
