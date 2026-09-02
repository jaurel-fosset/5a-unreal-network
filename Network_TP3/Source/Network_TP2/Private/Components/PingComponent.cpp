// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PingComponent.h"

#include "Network_TP2/LogNetwork.h"


UPingComponent::UPingComponent()
{
	SetIsReplicatedByDefault(true);
}

double UPingComponent::GetPing() const
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

double UPingComponent::GetCurrentTime()
{
	return LastServerTime + (GetWorld()->GetTimeSeconds() - LastServerTimeCorrespondingLocalTime);
}

void UPingComponent::Server_SyncToServerTime_Implementation()
{
	Client_ReceiveServerTime(GetWorld()->GetTimeSeconds());
}


void UPingComponent::BeginPlay()
{
	Super::BeginPlay();

	
	if (HasAuthority())
		return;
	
	GetWorld()->GetTimerManager()
		.SetTimer(PingTickTimer, this, &UPingComponent::PingTick, 0.1f, true);

	UE_LOG(LogNetworkTP, Display, TEXT("Owner : %s"), *GetOwner()->GetName());
}

bool UPingComponent::HasAuthority() const
{
	return GetOwner()->HasAuthority();
}

void UPingComponent::PingTick()
{
	if (HasAuthority())
		return;
	
	PingStartingTime[CurrentPingIndex] = GetWorld()->GetTimeSeconds();
	Server_Ping(CurrentPingIndex);

	CurrentPingIndex = PingStartingTime.GetNextIndex(CurrentPingIndex);
}

void UPingComponent::Client_ReceiveServerTime_Implementation(double ServerTime)
{
	double Ping = GetPing();
	SyncToServerTimeDelegate.Broadcast(ServerTime + Ping);
	LastServerTime = ServerTime;
	LastServerTimeCorrespondingLocalTime = GetWorld()->GetTimeSeconds();
}

void UPingComponent::Client_Pong_Implementation(const uint8 PingIndex)
{
	double CurrentTime = GetWorld()->GetTimeSeconds();
	const double RoundTripTime = CurrentTime - PingStartingTime[PingIndex];
	const double Ping = RoundTripTime / 2;
	ReceivedPing[PingIndex] = Ping;
}

void UPingComponent::Server_Ping_Implementation(const uint8 PingIndex)
{
	Client_Pong(PingIndex);
}
