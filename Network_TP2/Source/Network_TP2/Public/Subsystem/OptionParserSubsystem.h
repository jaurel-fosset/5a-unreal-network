// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BlueprintType/ConnectionOptions.h"
#include "OptionParserSubsystem.generated.h"

UENUM()
enum class ParseOptionResult
{
	Success,
	NoSkinKey,
	NoCorrespondingSkinTag,
};

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API UOptionParserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FString MakeOptionString(FConnectionOptions Options);

	UFUNCTION(BlueprintCallable, meta=(ExpandEnumAsExecs="ReturnValue"))
	static ParseOptionResult TryParseOptionString(const FString& Options, FConnectionOptions& OutOptions);
};
