// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/StaticSkinComponent.h"

#include "GameplayTagContainer.h"


// Sets default values for this component's properties
UStaticSkinComponent::UStaticSkinComponent()
{
}

void UStaticSkinComponent::SetupSkin(const FGameplayTag NewSkin)
{
	SetupSkinBlueprintEvent(NewSkin);
}
