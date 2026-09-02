// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/PlayerState/PlayerStateGame.h"

#include "Components/PingComponent.h"
#include "Components/Player/StaticSkinComponent.h"
#include "Net/UnrealNetwork.h"

APlayerStateGame::APlayerStateGame()
{
	SkinState = CreateDefaultSubobject<UStaticSkinState>(TEXT("SkinState"));
}

TObjectPtr<UStaticSkinState> APlayerStateGame::GetComponentSkin() const
{
	return SkinState;
}
