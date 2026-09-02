// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintType/ServerRewindInfo.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "TemplateType/TGoodCircularBuffer.h"
#include "ServerRewind.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UServerRewind : public UActorComponent
{
	GENERATED_BODY()

public:
	UServerRewind();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void DrawDebugCapsules(float HalfHeight, float Radius) const;
	
	UFUNCTION(BlueprintCallable)
	FServerRewindInfo GetRewindInfo(const double Timestamp);

	UFUNCTION(BlueprintCallable)
	void ResizeRewindBuffer(uint8 NewSize);

	void SetupPlayerForRewind(const double Timestamp);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	uint32 BufferCapacity = 50;

private:
	uint32 BufferIndex = 0;
	TGoodCircularBuffer<FServerRewindInfo> RewindBuffer = TGoodCircularBuffer<FServerRewindInfo>(BufferCapacity, FServerRewindInfo());

	UFUNCTION(Client, Unreliable)
	void SendRewindInfoDebug(FServerRewindInfo Info);

	bool HasAuthority();
};
