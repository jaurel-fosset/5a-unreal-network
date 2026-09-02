// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateGame.h"

#include "Net/UnrealNetwork.h"

APlayerStateGame::APlayerStateGame()
{
	bReplicates = true;
}

void APlayerStateGame::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerStateGame, Skin);
}

FGameplayTag APlayerStateGame::GetSkin() const
{
	return Skin;
}

void APlayerStateGame::SetupSkin(const FGameplayTag NewSkin)
{
	Skin = NewSkin;
}
