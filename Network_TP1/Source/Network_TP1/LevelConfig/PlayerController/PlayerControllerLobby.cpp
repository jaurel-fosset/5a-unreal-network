// Copyright Epic Games, Inc. All Rights Reserved.


#include "PlayerControllerLobby.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

APlayerControllerLobby::APlayerControllerLobby()
	: ReceivedPing(ReceivedPingCircularBufferSize, 0), PingStartingTime(ReceivedPingCircularBufferSize, 0)
{
}

double APlayerControllerLobby::GetPing()
{
	TArray<double> Pings = ReceivedPing.GetRaw();
	Pings.Sort();
	Pings[0] = 0.;
	Pings[Pings.Num() - 1] = 0.;
	
	double AveragePing = 0;
	for (const double Ping : Pings)
	{
		AveragePing += Ping;
	}
	AveragePing /= Pings.Num() - 2;

	return AveragePing;
}

double APlayerControllerLobby::GetCurrentTime()
{
	return LastServerTime + (GetWorld()->GetTimeSeconds() - LastServerTimeCorrespondingLocalTime);
}

void APlayerControllerLobby::Server_SyncToServerTime_Implementation()
{
	Client_ReceiveServerTime(GetWorld()->GetTimeSeconds());
}


void APlayerControllerLobby::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void APlayerControllerLobby::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		GetWorld()->GetTimerManager()
			.SetTimer(PingTickTimer, this, &APlayerControllerLobby::PingTick, 0.001f, true);
	}
}


void APlayerControllerLobby::Server_Ping_Implementation(const int8 PingId)
{
	Client_Pong(PingId);
}

void APlayerControllerLobby::Client_Pong_Implementation(const int8 PingId)
{
	double RoundTripTime = GetWorld()->GetTimeSeconds() - PingStartingTime[PingId];
	double Ping = RoundTripTime / 2;
	ReceivedPing[PingId] = Ping;
}

void APlayerControllerLobby::PingTick()
{
	PingStartingTime[CurrentPingIndex] = GetWorld()->GetTimeSeconds();
	Server_Ping(CurrentPingIndex);

	CurrentPingIndex = PingStartingTime.GetNextIndex(CurrentPingIndex + 1);
}

void APlayerControllerLobby::Client_ReceiveServerTime_Implementation(double ServerTime)
{
	double Ping = GetPing();
	SyncToServerTimeDelegate.Broadcast(ServerTime + Ping);
	LastServerTime = ServerTime;
	LastServerTimeCorrespondingLocalTime = GetWorld()->GetTimeSeconds();
}
