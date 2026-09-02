#pragma once
#include "GameplayTagContainer.h"
#include "ConnectionOptions.generated.h"



USTRUCT(BlueprintType)
struct NETWORK_TP1_API FConnectionOptions
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Skin;

	FConnectionOptions();
	explicit FConnectionOptions(FGameplayTag Skin);
};
