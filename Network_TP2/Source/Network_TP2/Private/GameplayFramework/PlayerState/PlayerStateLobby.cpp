// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_TP2/Public/GameplayFramework/PlayerState/PlayerStateLobby.h"

#include "GameplayFramework/PlayerState/PlayerStateGame.h"
#include "Network_TP2/LogNetwork.h"


APlayerStateLobby::APlayerStateLobby()
{
	bReplicates = true;
	SkinState = CreateDefaultSubobject<UDynamicSkinState>(TEXT("SkinState"));
}

TObjectPtr<UDynamicSkinState> APlayerStateLobby::GetComponentSkin() const
{
	return SkinState;
}

void APlayerStateLobby::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	const APlayerStateGame* PlayerStateGame = Cast<APlayerStateGame>(PlayerState);
	if (PlayerStateGame == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[APlayerStateLobby::CopyProperties] New Player State must be APlayerStateGame"));
		return;
	}

	const FGameplayTag SkinTag = GetComponentSkin()->GetSkin();
	PlayerStateGame->GetComponentSkin()->Server_SetupSkin(SkinTag);
}
