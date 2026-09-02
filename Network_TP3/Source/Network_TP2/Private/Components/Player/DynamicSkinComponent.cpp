// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_TP2/Public/Components/Player/DynamicSkinComponent.h"

#include "GameplayFramework/PlayerState/PlayerStateLobby.h"


// Sets default values for this component's properties
UDynamicSkinComponent::UDynamicSkinComponent()
{
}

void UDynamicSkinComponent::ChangeSkin(const FGameplayTag NewSkin)
{
	OnSkinChange.Broadcast(NewSkin);
}
