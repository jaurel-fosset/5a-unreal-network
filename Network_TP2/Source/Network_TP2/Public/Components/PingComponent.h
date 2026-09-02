// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateType/TGoodCircularBuffer.h"
#include "PingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSyncedToServerTimeSignature, double, ServerTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UPingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPingComponent();

	UFUNCTION(BlueprintCallable)
	double GetPing() const;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SyncToServerTime();

	UFUNCTION(BlueprintCallable)
	double GetCurrentTime();

	

protected:
	UPROPERTY(BlueprintAssignable, Category = "Ping")
	FOnSyncedToServerTimeSignature SyncToServerTimeDelegate;
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, Category="Ping")
	double LastServerTime = 0.0;
	UPROPERTY(VisibleInstanceOnly, Category="Ping")
	double LastServerTimeCorrespondingLocalTime = 0.0;

	int8 ReceivedPingCircularBufferSize = 10;
	uint32 CurrentPingIndex = 0;
	TGoodCircularBuffer<double> ReceivedPing = TGoodCircularBuffer<double>(ReceivedPingCircularBufferSize);
	TGoodCircularBuffer<double> PingStartingTime = TGoodCircularBuffer<double>(ReceivedPingCircularBufferSize);
	FTimerHandle PingTickTimer;
	
	bool HasAuthority() const;
	void PingTick();
	

	UFUNCTION(Server, Unreliable)
	void Server_Ping(const uint8 PingIndex);
	UFUNCTION(Client, Unreliable)
	void Client_Pong(const uint8 PingIndex);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveServerTime(double ServerTime);
};
