// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLostWidget.generated.h"

class UButton;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class SLASH_API UPlayerLostWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeAnim;
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* RestartGameBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;

private:
	UFUNCTION()
	void RestartGame();
	UFUNCTION()
	void BackMainMenu();
	UFUNCTION()
	void QuitGame();

	//다른곳에 설정해도 되지만 일단 여기.
	UPROPERTY(EditDefaultsOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> LobbyWorld;
	UPROPERTY(EditDefaultsOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> BattleWorld;
};
