// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DynamicSkinComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkinChangeSignature, const FGameplayTag, NewSkin);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UDynamicSkinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDynamicSkinComponent();

	UFUNCTION(BlueprintCallable)
	void ChangeSkin(const FGameplayTag NewSkin);

protected:
	UPROPERTY(BlueprintAssignable, Category="Skin", meta=(DisplayName="Skin Changed"))
	FOnSkinChangeSignature OnSkinChange;
};
