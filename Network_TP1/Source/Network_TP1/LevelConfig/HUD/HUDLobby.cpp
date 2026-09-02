// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDLobby.h"

DEFINE_LOG_CATEGORY(HUDLog);

void AHUDLobby::RemoveCurrentMenu()
{
	if (CurrentMenu == nullptr) return;

	GetOwningPlayerController()->SetShowMouseCursor(false);
	
	CurrentMenu->RemoveFromParent();
	CurrentMenu = nullptr;
}

void AHUDLobby::SwitchToMenu(TSubclassOf<class UUserWidget> WidgetClass)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	checkf(PlayerController != nullptr, TEXT("Player controller of HUDLobby is null"));
	
	UUserWidget* NewMenu = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (NewMenu == nullptr)
	{
		UE_LOG(HUDLog, Error, TEXT("HUDLobby.SwitchToMenu : Could not create the Widget for the new menu"));
		return;
	}

	if (CurrentMenu != nullptr)
	{
		CurrentMenu->RemoveFromParent();
	}
	else
	{
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
	
	NewMenu->AddToViewport();
	CurrentMenu = NewMenu;
}

UUserWidget* AHUDLobby::GetCurrentMenu() const
{
	return CurrentMenu.Get();
}
