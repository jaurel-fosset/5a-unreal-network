// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_TP2/Public/GameplayFramework/Pawn/CharacterLobby.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayFramework/PlayerState/PlayerStateLobby.h"
#include "Network_TP2/LogNetwork.h"


ACharacterLobby::ACharacterLobby()
{
	ThirdPersonMovementComponent = CreateDefaultSubobject<UThirdPersonMovementComponent>("ThirdPersonMovementComponent");
	SkinComponent = CreateDefaultSubobject<UDynamicSkinComponent>("SkinComponent");

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
}

void ACharacterLobby::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SyncWithPlayerState();
}


void ACharacterLobby::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	ThirdPersonMovementComponent->SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterLobby::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	SyncWithPlayerState();
}

TObjectPtr<APlayerStateLobby> ACharacterLobby::GetPlayerStateLobby()
{
	if (GetPlayerState() == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterLobby::GetPlayerStateLobby] No player state"));
		return nullptr;
	}

	TObjectPtr<APlayerStateLobby> PlayerStateLobby = Cast<APlayerStateLobby>(GetPlayerState());
	if (PlayerStateLobby == nullptr)
	{
		UE_LOG(LogNetworkTP, Display, TEXT("[ACharacterLobby::GetPlayerStateLobby] Wrong player state type"));
		return nullptr;
	}
	
	return PlayerStateLobby;
}

void ACharacterLobby::SyncWithPlayerState()
{
	const TObjectPtr<APlayerStateLobby> PlayerStateLobby = GetPlayerStateLobby();
	if (PlayerStateLobby == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[ACharacterLobby::Restart] Could not get player state"));
		return;
	}
	
	SkinComponent->ChangeSkin(PlayerStateLobby->GetComponentSkin()->GetSkin());
	PlayerStateLobby->GetComponentSkin()->OnSkinChanged.AddUniqueDynamic(SkinComponent, &UDynamicSkinComponent::ChangeSkin);
}

