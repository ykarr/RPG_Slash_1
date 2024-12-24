// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/PlayerLostWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
void UPlayerLostWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RestartGameBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::RestartGame);
	MainMenuBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::BackMainMenu);
	QuitBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::QuitGame);
}

void UPlayerLostWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeAnim);
	if (GetOwningPlayer()) {
		FInputModeUIOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
}

void UPlayerLostWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UPlayerLostWidget::RestartGame()
{
	if (GetOwningPlayer()) {
		FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	ensureMsgf(BattleWorld!=nullptr,TEXT("Fill BattleWorld in PlayerLostWidget"));
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), BattleWorld);
	RemoveFromParent();
	
}

void UPlayerLostWidget::BackMainMenu()
{
	ensureMsgf(LobbyWorld !=nullptr,TEXT("Fill LobbyWorld in PlayerLostWidget"));
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LobbyWorld);
	RemoveFromParent();
}

void UPlayerLostWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
