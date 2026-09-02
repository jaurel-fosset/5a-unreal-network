// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ServerRewind.h"

#include "VectorTypes.h"
#include "GameplayFramework/Pawn/CharacterGame.h"
#include "Network_TP2/LogNetwork.h"


UServerRewind::UServerRewind()
{
	PrimaryComponentTick.bCanEverTick = true;
	RewindBuffer = TGoodCircularBuffer<FServerRewindInfo>(BufferCapacity);
}


void UServerRewind::BeginPlay()
{
	Super::BeginPlay();
}

void UServerRewind::SendRewindInfoDebug_Implementation(FServerRewindInfo Info)
{
	RewindBuffer[BufferIndex] = Info;
	BufferIndex = RewindBuffer.GetNextIndex(BufferIndex);
}

bool UServerRewind::HasAuthority()
{
	return GetOwnerRole() == ROLE_Authority;
}


void UServerRewind::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasAuthority())
		return;
	
	const TObjectPtr<AActor> Owner = GetOwner();

	FServerRewindInfo RewindInfo = FServerRewindInfo();
	RewindInfo.Timestamp = GetWorld()->GetTimeSeconds();
	RewindInfo.PlayerPosition = Owner->GetActorLocation();
	RewindInfo.PlayerRotation = Owner->GetActorRotation().Quaternion();

	RewindBuffer[BufferIndex] = RewindInfo;
	BufferIndex = RewindBuffer.GetNextIndex(BufferIndex);

	// SendRewindInfoDebug(RewindInfo);
}

void UServerRewind::DrawDebugCapsules(float HalfHeight, float Radius) const
{
	const TObjectPtr<UWorld> World = GetWorld();
	if (World == nullptr)
		return;

	for (FServerRewindInfo RewindInfo : RewindBuffer)
	{
		DrawDebugCapsule(World, RewindInfo.PlayerPosition, HalfHeight, Radius, FQuat::Identity,
				FColor::Green, false, -1.f, 0, 0.5f);
	}
}

FServerRewindInfo UServerRewind::GetRewindInfo(const double Timestamp)
{
	const uint32 Index = RewindBuffer.FindLastByPredicate(BufferIndex, [Timestamp](const FServerRewindInfo& RewindInfo)
		{
			return Timestamp >= RewindInfo.Timestamp;
		});

	const FServerRewindInfo LowerBound = RewindBuffer[Index];
	const FServerRewindInfo UpperBound = RewindBuffer[RewindBuffer.GetNextIndex(Index)];

	const double t = (Timestamp - LowerBound.Timestamp) / (UpperBound.Timestamp - LowerBound.Timestamp);
	const FVector ExtrapolatedPosition = UE::Geometry::Lerp(LowerBound.PlayerPosition, UpperBound.PlayerPosition, t);
	const FQuat ExtrapolatedRotation = FQuat::Slerp(LowerBound.PlayerRotation, UpperBound.PlayerRotation, t);

	return FServerRewindInfo(Timestamp, ExtrapolatedPosition, ExtrapolatedRotation);
}

void UServerRewind::ResizeRewindBuffer(const uint8 NewSize)
{
	RewindBuffer.SetNum(NewSize);
}

void UServerRewind::SetupPlayerForRewind(const double Timestamp)
{
	if (!HasAuthority())
		return;

	const TObjectPtr<UWorld> World = GetWorld();
	if (World == nullptr)
		return;

	// TODO : Replace with player array in GameMode since we are on the server
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		const APlayerController* PlayerController = Iterator->Get();
		const ACharacterGame* Character = Cast<ACharacterGame>(PlayerController->GetPawn());
		if (Character == nullptr)
		{
			UE_LOG(LogNetworkTP, Error, TEXT("[UServerRewind::SetupPlayerForRewind] Not all Character are of"
									"the expected type"))
			continue;
		}

		Character->SetupServerRewindCapsule(Timestamp);
	}
}
