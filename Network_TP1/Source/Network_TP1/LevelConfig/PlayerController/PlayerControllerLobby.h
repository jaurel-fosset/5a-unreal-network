// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TemplateType/TGoodCircularBuffer.h"
#include "PlayerControllerLobby.generated.h"

class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSyncedToServerTimeSignature, double, ServerTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoreThanTwoPlayerSignature);

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class APlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerLobby();
	
	double GetPing();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SyncToServerTime();

	UFUNCTION(BlueprintCallable)
	double GetCurrentTime();

	UPROPERTY(BlueprintAssignable, Category = "Ping")
	FOnSyncedToServerTimeSignature SyncToServerTimeDelegate;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	virtual void SetupInputComponent() override;
	void BeginPlay() override;

private:
	double LastServerTime = 0.0;
	double LastServerTimeCorrespondingLocalTime = 0.0;
	
	int8 ReceivedPingCircularBufferSize = 10;
	uint32 CurrentPingIndex = 0;
	TGoodCircularBuffer<double> ReceivedPing;
	TGoodCircularBuffer<double> PingStartingTime;
	
	UFUNCTION(Server, Unreliable)
	void Server_Ping(const int8 PingId);

	UFUNCTION(Client, Unreliable)
	void Client_Pong(const int8 PingId);

	FTimerHandle PingTickTimer;
	void PingTick();
	
	UFUNCTION(Client, Reliable)
	void Client_ReceiveServerTime(double ServerTime);
};
