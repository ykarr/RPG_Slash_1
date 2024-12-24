// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SLASH_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void EndGame();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartBtn;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> QuitBtn;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> SurvivalMap;
};



