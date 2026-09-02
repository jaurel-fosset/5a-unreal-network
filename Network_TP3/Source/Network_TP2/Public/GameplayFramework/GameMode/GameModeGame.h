// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeGame.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API AGameModeGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeGame();
	
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal = L"") override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
