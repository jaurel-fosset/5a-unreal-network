// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ServerRewind.h"

#include "VectorTypes.h"
#include "GameFramework/GameState.h"
#include "GameplayFramework/GameState/GameStateGame.h"
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
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[UServerRewind::DrawDebugCapsules] Could not find UWorld,"
								   "potential cause : no owning actor, owning actor not spawned in a world"
									"or live coding issue :["));
		return;
	}
		
	const TObjectPtr<AGameStateGame> GameStateGame = World->GetGameState<AGameStateGame>();
	if (GameStateGame == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[UServerRewind::DrawDebugCapsules] Unexpected GameState type"));
		return;
	}


	if (!GameStateGame->GetShowDebug())
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Red, TEXT("Show Debug = false"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Show Debug = true"));

	for (FServerRewindInfo RewindInfo : RewindBuffer)
	{
		DrawDebugCapsule(World, RewindInfo.PlayerPosition, HalfHeight, Radius, FQuat::Identity,
				FColor::Green, false, -1.f, 0, 0.5f);
	}
}

FServerRewindInfo UServerRewind::GetRewindInfo(const double Timestamp)
{
	double LowestTimestamp = std::numeric_limits<double>::max();
	uint32 LowestTimestampIndex = 0;
	for (uint32 Index = RewindBuffer.GetPreviousIndex(BufferIndex); Index != BufferIndex; Index = RewindBuffer.GetPreviousIndex(Index))
	{
		if (LowestTimestamp > RewindBuffer[Index].Timestamp)
		{
			LowestTimestamp = RewindBuffer[Index].Timestamp;
			LowestTimestampIndex = Index;
		}
		if (Timestamp > RewindBuffer[Index].Timestamp)
		{
			LowestTimestamp = RewindBuffer[Index].Timestamp;
			LowestTimestampIndex = Index;
			break;
		}
	}
	

	const FServerRewindInfo LowerBound = RewindBuffer[LowestTimestampIndex];
	const FServerRewindInfo UpperBound = RewindBuffer[RewindBuffer.GetNextIndex(LowestTimestampIndex)];

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
