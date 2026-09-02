// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeMain();
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
};



