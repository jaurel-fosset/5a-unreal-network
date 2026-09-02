// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/HUD.h"
#include "SimpleHUD.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API ASimpleHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Remove Current Menu"))
	void RemoveCurrentMenu();
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Switch to Menu"))
	void SwitchToMenu(TSubclassOf<class UUserWidget> WidgetClass);
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Get Current Menu"))
	UUserWidget* GetCurrentMenu() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD")
	TSubclassOf<class UUserWidget> HUD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD")
	int32 HudZIndex = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD")
	int32 MenuZIndex = 1;

	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentMenu;
};
