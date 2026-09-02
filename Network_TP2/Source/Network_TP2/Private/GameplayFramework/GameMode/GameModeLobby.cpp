// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/GameMode/GameModeLobby.h"
#include "GameplayFramework/GameState/GameStateLobby.h"
#include "GameplayFramework/PlayerController/PlayerControllerLobby.h"
#include "GameplayFramework/PlayerState/PlayerStateLobby.h"
#include "Subsystem/OptionParserSubsystem.h"

#include "GameplayTagContainer.h"
#include "Network_TP2/LogNetwork.h"
#include "Subsystem/SimpleNetworkSubsystem.h"

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

	const APlayerStateLobby* PlayerStateLobby = Cast<APlayerStateLobby>(NewPlayerController->PlayerState);
	if (PlayerStateLobby == nullptr)
		return FString(TEXT("Player state is not a PlayerStateLobby"));
	

	FConnectionOptions ParsedOptions;
	if (FString ErrorMessage; !TryParseOptions(Options, ErrorMessage, ParsedOptions))
	{
		UE_LOG(GameModeLobby, Display, TEXT("%s"), *ErrorMessage);
		return ErrorMessage;
	}

	const FGameplayTag SkinTag = ParsedOptions.Skin;
	checkf(SkinTag.IsValid(), TEXT("The parser should output a valid gameplay tag"));
	UE_LOG(LogNetworkTP, Display, TEXT("SkinTag : %s"), *SkinTag.ToString());
	
	PlayerStateLobby->GetComponentSkin()->SetSkin(SkinTag);
	
	return FString();
}

void AGameModeLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetNumPlayers() >= 2)
	{
		const TObjectPtr<AGameStateLobby> GameStateLobby = Cast<AGameStateLobby>(GameState);
		check(GameStateLobby != nullptr)
		if (GameStateLobby->TimerEnd != -1.0)
			return;
		
		const double TimerEnd = GetWorld()->GetTimeSeconds() + TimerDuration;
		GameStateLobby->TimerEnd = TimerEnd;
		GameStateLobby->OnStartTimerDelegate.Broadcast(TimerEnd);

		GetWorld()->GetTimerManager()
			.SetTimer(TimerEndHandle, this, &AGameModeLobby::OnTimerEnd, TimerDuration, false);
	}
}

bool AGameModeLobby::TryParseOptions(const FString& Options, FString& OutErrorMessage,
	FConnectionOptions& OutConnectionOptions)
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

