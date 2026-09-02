// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/HUD.h"
#include "HUDLobby.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(HUDLog, Display, All);

/**
 * 
 */
UCLASS()
class NETWORK_TP1_API AHUDLobby : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Remove Current Menu"))
	void RemoveCurrentMenu();
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Switch to Menu"))
	void SwitchToMenu(TSubclassOf<class UUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get Current Menu"))
	UUserWidget* GetCurrentMenu() const;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentMenu;
};
