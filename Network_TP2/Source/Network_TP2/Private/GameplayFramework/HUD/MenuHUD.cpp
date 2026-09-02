// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/HUD/MenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Network_TP2/LogNetwork.h"


void AMenuHUD::PushMenu(TSubclassOf<UUserWidget> NewMenuClass)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	
	
	UUserWidget* NewMenu = CreateWidget<UUserWidget>(PlayerController, NewMenuClass);
	if (NewMenu == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[HUDLobby::SwitchToMenu] Could not create the Widget for the new menu"));
		return;
	}

	if (WidgetsStack.Num() != 0)
	{
		WidgetsStack.Last()->RemoveFromParent();
	}
	
	NewMenu->AddToViewport();
	WidgetsStack.Add(NewMenu);
}

void AMenuHUD::PopMenu()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (WidgetsStack.Num() == 0)
		return;
	
	UUserWidget* MenuToRemove = WidgetsStack.Pop();
	MenuToRemove->RemoveFromParent();
	WidgetsStack.Last()->AddToViewport();
}

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogNetworkTP, Error, TEXT("[HUDLobby::BeginPlay] Could not get our owning controller,"
								   "not setting the mouse cursor to show has a consequence"));
		return;
	}
	
	PlayerController->SetShowMouseCursor(true);
	PushMenu(StartMenu);
}
