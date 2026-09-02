#pragma once
#include "ServerRewindInfo.generated.h"

USTRUCT(BlueprintType)
struct FServerRewindInfo
{
	GENERATED_BODY()

	double Timestamp;
	
	FVector PlayerPosition = FVector(0, 0, 0);
	FQuat PlayerRotation = FQuat::Identity;
};
