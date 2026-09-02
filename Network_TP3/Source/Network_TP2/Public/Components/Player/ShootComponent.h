// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShootComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NETWORK_TP2_API UShootComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	TOptional<FHitResult> Shoot(const UWorld* World, ECollisionChannel CollisionChannel,
	                         FLinearColor NoHit, FLinearColor Hit, const FVector& Position, const FRotator& Rotation) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	float MaxShootDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Shoot | Debug")
	float DebugLineDisplayTime = 5.f;
};
