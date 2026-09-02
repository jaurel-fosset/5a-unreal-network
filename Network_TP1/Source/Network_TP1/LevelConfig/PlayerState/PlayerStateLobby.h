// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "LevelConfig/Pawn/CharacterLobby.h"
#include "PlayerStateLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkinChangedSignature, FGameplayTag, NewSkin);

/**
 * 
 */
UCLASS()
class NETWORK_TP1_API APlayerStateLobby : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerStateLobby();

	UFUNCTION(Server, Reliable, BlueprintCallable, meta=(DisplayName="Set Skin (Client)"))
	void Server_SetSkin(const FGameplayTag NewSkin);

	UFUNCTION(BlueprintPure, Category="Skin", meta=(BlueprintInternalUseOnly=true))
	FGameplayTag GetSkin() const;
	UFUNCTION(BlueprintCallable, Category="Skin", meta=(BlueprintInternalUseOnly=true))
	void SetSkin(const FGameplayTag NewSkin);

	UPROPERTY(BlueprintAssignable, Category="Skin")
	FOnSkinChangedSignature OnSkinChanged;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;



private:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter="GetSkin", BlueprintSetter="SetSkin",
		ReplicatedUsing=OnRep_Skin, Category="Skin", meta=(AllowPrivateAccess))
	FGameplayTag Skin;


	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	UFUNCTION()
	void OnRep_Skin() const;
};
