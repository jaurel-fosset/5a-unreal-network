// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/ShootComponent.h"

#include "KismetTraceUtils.h"


TOptional<FHitResult> UShootComponent::Shoot(const UWorld* World, const ECollisionChannel CollisionChannel,
	FLinearColor NoHit, FLinearColor Hit, const FVector& Position, const FRotator& Rotation) const
{
	FHitResult RaycastResult;
	
	if (World == nullptr)
		return TOptional<FHitResult>();

	const TObjectPtr<AActor> Owner = GetOwner();

	FCollisionQueryParams RaycastParameter;
	RaycastParameter.AddIgnoredActor(Owner);

	const FVector RaycastStart = Position;
	const FVector RaycastEnd = Position + Rotation.Vector()*MaxShootDistance;

	
	const bool bHasHit = World->LineTraceSingleByChannel(RaycastResult, RaycastStart, RaycastEnd,
		CollisionChannel, RaycastParameter);
	
	DrawDebugLineTraceSingle(World, RaycastStart, RaycastEnd, EDrawDebugTrace::ForDuration, bHasHit,
		RaycastResult, NoHit, Hit, DebugLineDisplayTime);

	if (!bHasHit)
		return TOptional<FHitResult>();
	
	return TOptional<FHitResult>(RaycastResult);
}

