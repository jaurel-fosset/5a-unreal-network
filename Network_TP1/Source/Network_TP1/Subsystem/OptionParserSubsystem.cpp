// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionParserSubsystem.h"

#include "Kismet/GameplayStatics.h"

FString UOptionParserSubsystem::MakeOptionString(const FConnectionOptions Options)
{
	FString OptionString;
	OptionString += "Skin=" + Options.Skin.ToString();
	return OptionString;
}

ParseOptionResult UOptionParserSubsystem::TryParseOptionString(const FString& Options, FConnectionOptions& OutOptions)
{
	const FString Skin = UGameplayStatics::ParseOption(Options, "Skin");
	if (Skin.IsEmpty())
	{
		return ParseOptionResult::NoSkinKey;
	}

	const FGameplayTag SkinTag = FGameplayTag::RequestGameplayTag(FName(*Skin), false);
	if (!SkinTag.IsValid())
	{
		return ParseOptionResult::NoCorrespondingSkinTag;
	}

	OutOptions = FConnectionOptions(SkinTag);
	return ParseOptionResult::Success;
}
