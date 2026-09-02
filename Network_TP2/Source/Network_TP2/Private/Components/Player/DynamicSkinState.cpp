// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/DynamicSkinState.h"

#include "Net/UnrealNetwork.h"


UDynamicSkinState::UDynamicSkinState()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UDynamicSkinState::Server_SetSkin_Implementation(const FGameplayTag NewSkin)
{
	SetSkin(NewSkin);
}

void UDynamicSkinState::SetSkin(const FGameplayTag NewSkin)
{
	Skin = NewSkin;
	OnSkinChanged.Broadcast(NewSkin);
}

FGameplayTag UDynamicSkinState::GetSkin() const
{
	return Skin;
}

void UDynamicSkinState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDynamicSkinState, Skin);
}

void UDynamicSkinState::OnRep_Skin() const
{
	OnSkinChanged.Broadcast(Skin);
}
