// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BlueprintType/ConnectionOptions.h"
#include "GameModeLobby.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameModeLobby, Display, All);


/**
 * 
 */
UCLASS()
class NETWORK_TP2_API AGameModeLobby : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeLobby();
	
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal = L"") override;

	virtual void PostLogin(APlayerController* NewPlayer) override;


protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTimerEnd();
	
private:
	UPROPERTY(EditDefaultsOnly)
	double TimerDuration;
	
	TMap<FUniqueNetIdRepl, FConnectionOptions> ConnectedClientOptions;

	static bool TryParseOptions(const FString& Options, FString& OutErrorMessage, FConnectionOptions& OutConnectionOptions);

	FTimerHandle TimerEndHandle;
	
};
