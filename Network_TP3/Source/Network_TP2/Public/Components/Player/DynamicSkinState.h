// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DynamicSkinState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkinChangedSignature, FGameplayTag, NewSkin);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UDynamicSkinState : public UActorComponent
{
	GENERATED_BODY()

public:
	UDynamicSkinState();
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Skin")
	void Server_SetSkin(const FGameplayTag NewSkin);

	UFUNCTION(BlueprintCallable, Category="Skin")
	void SetSkin(const FGameplayTag NewSkin);
	
	UFUNCTION(BlueprintPure, Category="Skin")
	FGameplayTag GetSkin() const;
	
	
	UPROPERTY(BlueprintAssignable, Category="Skin")
	FOnSkinChangedSignature OnSkinChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Skin)
	FGameplayTag Skin;
	
	UFUNCTION()
	void OnRep_Skin() const;
};
