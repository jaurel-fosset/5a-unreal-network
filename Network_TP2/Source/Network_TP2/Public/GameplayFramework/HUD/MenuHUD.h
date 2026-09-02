// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_TP2_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="HUD | Menu")
	void PushMenu(TSubclassOf<UUserWidget> NewMenu);

	UFUNCTION(BlueprintCallable, Category="HUD | Menu")
	void PopMenu();

protected:
	UPROPERTY(EditDefaultsOnly, Category="HUD | Menu")
	TSubclassOf<UUserWidget> StartMenu;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetsStack;
};
