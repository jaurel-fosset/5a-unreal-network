// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/ShootComponent.h"

#include "KismetTraceUtils.h"


TObjectPtr<AActor> UShootComponent::Shoot(const UWorld* World, const ECollisionChannel CollisionChannel,
	FLinearColor NoHit, FLinearColor Hit, const FVector& Position, const FRotator& Rotation) const
{
	if (World == nullptr)
		return nullptr;

	const TObjectPtr<AActor> Owner = GetOwner();

	FCollisionQueryParams RaycastParameter;
	RaycastParameter.AddIgnoredActor(Owner);

	const FVector RaycastStart = Position;
	const FVector RaycastEnd = Position + Rotation.Vector()*MaxShootDistance;
	
	FHitResult RaycastResult;
	const bool bHasHit = World->LineTraceSingleByChannel(RaycastResult, RaycastStart, RaycastEnd,
		CollisionChannel, RaycastParameter);
	
	DrawDebugLineTraceSingle(World, RaycastStart, RaycastEnd, EDrawDebugTrace::ForDuration, bHasHit,
		RaycastResult, NoHit, Hit, DebugLineDisplayTime);

	if (bHasHit)
		return RaycastResult.GetActor();
	return nullptr;
}

