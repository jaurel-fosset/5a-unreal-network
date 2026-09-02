// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateLobby.h"

#include "LogNetworkTP.h"
#include "PlayerStateGame.h"
#include "LevelConfig/Pawn/CharacterLobby.h"
#include "Net/UnrealNetwork.h"


APlayerStateLobby::APlayerStateLobby()
{
	bReplicates = true;
}

void APlayerStateLobby::Server_SetSkin_Implementation(const FGameplayTag NewSkin)
{
	SetSkin(NewSkin);
}

FGameplayTag APlayerStateLobby::GetSkin() const
{
	return Skin;
}

void APlayerStateLobby::SetSkin(const FGameplayTag NewSkin)
{
	Skin = NewSkin;
	OnSkinChanged.Broadcast(Skin);
}

void APlayerStateLobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerStateLobby, Skin);
}

void APlayerStateLobby::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	APlayerStateGame* PlayerStateGame = Cast<APlayerStateGame>(PlayerState);
	check(PlayerStateGame != nullptr);

	PlayerStateGame->SetupSkin(Skin);
}

void APlayerStateLobby::OnRep_Skin() const
{
	OnSkinChanged.Broadcast(Skin);
}
