// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UWidgetAnimation;
class UButton;
/**
 * 
 */
UCLASS()
class SLASH_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(Transient,meta = (BindWidgetAnim))
	UWidgetAnimation* MenuAnim;
	UPROPERTY(meta=(BindWidget))
	UButton* BackGameBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* MenuBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitBtn;

private:
	UFUNCTION()
	void BacktoGame();
	UFUNCTION()
	void BacktoMenu();
	UFUNCTION()
	void QuitGame();
};
