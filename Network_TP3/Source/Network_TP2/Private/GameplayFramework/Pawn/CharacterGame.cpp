// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/Pawn/CharacterGame.h"

#include "Components/CapsuleComponent.h"
#include "Components/Player/DynamicSkinComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Network_TP2/LogNetwork.h"


ACharacterGame::ACharacterGame()
{
	ThirdPersonMovementComponent = CreateDefaultSubobject<UThirdPersonMovementComponent>("ThirdPersonMovementComponent");
	SkinComponent = CreateDefaultSubobject<UStaticSkinComponent>("SkinComponent");
	ServerRewind = CreateDefaultSubobject<UServerRewind>("ServerRewind");
	

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	ServerRewindCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ServerRewindCapsule"));
	ServerRewindCapsule->SetCollisionObjectType(ServerRewindCollisionChannel);
	ServerRewindCapsule->SetupAttachment(RootComponent);

	ShootComponent = CreateDefaultSubobject<UShootComponent>("ShootComponent");
	ShootComponent->SetupAttachment(RootComponent);
}

void ACharacterGame::SetupServerRewindCapsule(const double Timestamp) const
{
	FServerRewindInfo Info = ServerRewind->GetRewindInfo(Timestamp);
	ServerRewindCapsule->SetWorldLocation(Info.PlayerPosition);
	ServerRewindCapsule->SetWorldRotation(Info.PlayerRotation);
}

void ACharacterGame::Server_ShootWithRewind_Implementation(const double Timestamp)
{
	ServerRewind->SetupPlayerForRewind(Timestamp);

	const FServerRewindInfo Info = ServerRewind->GetRewindInfo(Timestamp);
	const TOptional<FHitResult> ShotResult = ShootComponent->Shoot(GetWorld(), ServerRewindCollisionChannel,
	                                                           FColor::Green, FColor::Red, Info.PlayerPosition, Info.PlayerRotation.Rotator());
	if (!ShotResult.IsSet())
		return;
	
	const TObjectPtr<AActor> ShotActor = ShotResult->GetActor();
	if (ShotActor == nullptr)
		return;
	
	ACharacterGame* ShotPlayer = Cast<ACharacterGame>(ShotActor);
	if (ShotPlayer == nullptr)
		return;

	DrawDebugCapsule(GetWorld(), ShotResult->GetComponent()->GetComponentLocation(),
		96.f, 42.f, FQuat::Identity, FColor::Green, false, 5.f);
	
	ShotPlayer->OnDeath();
}

void ACharacterGame::Shoot()
{
	const TObjectPtr<UWorld> World = GetWorld();
	if (World == nullptr)
		return;

	const TObjectPtr<APlayerControllerGame> PlayerControllerGame = GetPlayerControllerGame();
	if (PlayerControllerGame == nullptr)
		return;
	
	const double CurrentTime = PlayerControllerGame->GetPingComponent()->GetCurrentTime();
	Server_ShootWithRewind(CurrentTime);
	
	TOptional<FHitResult> ShotResult = ShootComponent->Shoot(World, ECC_Pawn,
	                                                           FColor::Blue, FColor::Cyan, GetActorLocation(), GetActorRotation());
	if (!ShotResult.IsSet())
		return;
	const TObjectPtr<AActor> ShotActor = ShotResult->GetActor();
	if (ShotActor == nullptr)
		return;
	
	ACharacterGame* ShotPlayer = Cast<ACharacterGame>(ShotActor);
	if (ShotPlayer == nullptr)
		return;
	
	ShotPlayer->OnDeathPredictionBlueprintEvent();
}

void ACharacterGame::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ServerRewind->DrawDebugCapsules(96.f, 42.f);
}

void ACharacterGame::Restart()
{
	Super::Restart();

	const TObjectPtr<APlayerStateGame> PlayerStateLobby = GetPlayerStateGame();
	if (PlayerStateLobby == nullptr)
		return;
	
	SkinComponent->SetupSkin(PlayerStateLobby->GetComponentSkin()->GetSkin());
}

void ACharacterGame::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	ThirdPersonMovementComponent->SetupPlayerInputComponent(PlayerInputComponent);
}

TObjectPtr<APlayerStateGame> ACharacterGame::GetPlayerStateGame() const
{
	if (GetPlayerState() == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterGame::GetPlayerStateGame] No player state"));
		return nullptr;
	}

	TObjectPtr<APlayerStateGame> PlayerStateLobby = Cast<APlayerStateGame>(GetPlayerState());
	if (PlayerStateLobby == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterGame::GetPlayerStateGame] Unexpected controller type"));
		return nullptr;
	}
	
	return PlayerStateLobby;
}

TObjectPtr<APlayerControllerGame> ACharacterGame::GetPlayerControllerGame() const
{
	if (Controller == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterGame::GetPlayerControllerGame] Pawn not currently controlled"));
		return nullptr;
	}

	TObjectPtr<APlayerControllerGame> PlayerControllerGame = Cast<APlayerControllerGame>(Controller);
	if (PlayerControllerGame == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterGame::GetPlayerControllerGame] Unexpected controller type"));
		return nullptr;
	}
	return PlayerControllerGame;
}

void ACharacterGame::OnDeath_Implementation()
{
	OnDeathBlueprintEvent();
}
