// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateLobby.h"

#include "Net/UnrealNetwork.h"


void AGameStateLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameStateLobby, TimerEnd);
}

void AGameStateLobby::OnRep_TimerEnd() const
{
	OnStartTimerDelegate.Broadcast(TimerEnd);
}
