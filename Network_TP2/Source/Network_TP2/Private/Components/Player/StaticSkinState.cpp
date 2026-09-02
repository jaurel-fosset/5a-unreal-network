// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/StaticSkinState.h"

#include "Net/UnrealNetwork.h"
#include "Network_TP2/LogNetwork.h"


// Sets default values for this component's properties
UStaticSkinState::UStaticSkinState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FGameplayTag UStaticSkinState::GetSkin() const
{
	return Skin;
}

void UStaticSkinState::Server_SetupSkin_Implementation(FGameplayTag NewSkin)
{
	SetupSkin(NewSkin);
}

void UStaticSkinState::SetupSkin(const FGameplayTag NewSkin)
{
	if(IsSkinSetup)
		return;

	if (!NewSkin.IsValid())
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[UStaticSkinState::SetupSkin] Skin is invalid]"));
		return;
	}
	
	IsSkinSetup = true;
	Skin = NewSkin;
}

void UStaticSkinState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStaticSkinState, Skin);
}


