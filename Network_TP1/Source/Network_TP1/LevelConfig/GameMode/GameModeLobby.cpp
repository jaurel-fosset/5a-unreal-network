// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeLobby.h"

#include "BlueprintType/ConnectionOptions.h"
#include "LevelConfig/GameState/GameStateLobby.h"
#include "LevelConfig/PlayerController/PlayerControllerLobby.h"
#include "LevelConfig/PlayerState/PlayerStateLobby.h"
#include "Subsystem/OptionParserSubsystem.h"

DEFINE_LOG_CATEGORY(GameModeLobby);


AGameModeLobby::AGameModeLobby()
{
	bUseSeamlessTravel = true;
}

FString AGameModeLobby::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                      const FString& Options, const FString& Portal)
{
	{
		FString ErrorMessage = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
		if (!ErrorMessage.IsEmpty())
			return ErrorMessage;
	}
	
	APlayerStateLobby* PlayerStateMain = Cast<APlayerStateLobby>(NewPlayerController->PlayerState);
	if (PlayerStateMain == nullptr)
		return FString(TEXT("Player state is not a PlayerStateMain"));
	
	
	UE_LOG(GameModeLobby, Display, TEXT("Connection options: %s"), *Options);

	FConnectionOptions ParsedOptions;
	if (FString ErrorMessage; !TryParseOptions(Options, ErrorMessage, ParsedOptions))
	{
		UE_LOG(GameModeLobby, Display, TEXT("%s"), *ErrorMessage);
		return ErrorMessage;
	}

	const FGameplayTag SkinTag = ParsedOptions.Skin;
	checkf(SkinTag.IsValid(), TEXT("The parser should output a valid gameplay tag"));
	UE_LOG(GameModeLobby, Display, TEXT("SkinTag: %s"), *Options);
	
	PlayerStateMain->SetSkin(SkinTag);
	
	return FString();
}

void AGameModeLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetNumPlayers() >= 2)
	{
		const TObjectPtr<AGameStateLobby> GameStateLobby = Cast<AGameStateLobby>(GameState);
		check(GameStateLobby != nullptr)
		if (GameStateLobby->TimerEnd != 0.0)
			return;
		
		const double TimerEnd = GetWorld()->GetTimeSeconds() + TimerDuration;
		GameStateLobby->TimerEnd = TimerEnd;
		GameStateLobby->OnStartTimerDelegate.Broadcast(TimerEnd);
	}
}

bool AGameModeLobby::TryParseOptions(const FString& Options, FString& OutErrorMessage, FConnectionOptions& OutConnectionOptions)
{
	OutErrorMessage = TEXT("");
	
	switch (UOptionParserSubsystem::TryParseOptionString(Options, OutConnectionOptions))
	{
	case ParseOptionResult::NoSkinKey:
		OutErrorMessage = TEXT("Options string malformed : could not parse for value of skin parameter");
		return false;
	case ParseOptionResult::NoCorrespondingSkinTag:
		OutErrorMessage = TEXT("Options string malformed : skin value does not correspond to a valid gameplay tag");
		return false;
	case ParseOptionResult::Success:
		return true;
	}

	autortfm_unreachable("All case should be handled and return, making the switch the end point of this function");
	return true;
}
