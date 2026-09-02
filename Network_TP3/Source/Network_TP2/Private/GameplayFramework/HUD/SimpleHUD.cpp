// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/HUD/SimpleHUD.h"

#include "Blueprint/UserWidget.h"
#include "Network_TP2/LogNetwork.h"

void ASimpleHUD::RemoveCurrentMenu()
{
	if (CurrentMenu == nullptr) return;

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController != nullptr)
		PlayerController->SetShowMouseCursor(false);
	
	CurrentMenu->RemoveFromParent();
	CurrentMenu = nullptr;
}

void ASimpleHUD::SwitchToMenu(const TSubclassOf<class UUserWidget> WidgetClass)
{
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[ASimpleHUD::SwitchToMenu] Provided Widget Class is null"));
		return;
	}
		
	
	APlayerController* PlayerController = GetOwningPlayerController();
	
	UUserWidget* NewMenu = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (NewMenu == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[ASimpleHUD::SwitchToMenu] Could not create the Widget for the new menu"));
		return;
	}

	if (CurrentMenu != nullptr)
	{
		CurrentMenu->RemoveFromParent();
	}

	if (PlayerController != nullptr && CurrentMenu == nullptr)
	{
		PlayerController->SetShowMouseCursor(true);
	}
	
	NewMenu->AddToPlayerScreen(HudZIndex);
	CurrentMenu = NewMenu;
}

UUserWidget* ASimpleHUD::GetCurrentMenu() const
{
	return CurrentMenu.Get();
}

void ASimpleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUD == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[ASimpleHUD::SwitchToMenu] HUD Widget Class is null"));
		return;
	}
	SwitchToMenu(HUD);
}
