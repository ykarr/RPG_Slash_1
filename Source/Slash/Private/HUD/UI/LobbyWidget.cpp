// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/LobbyWidget.h"
#include "Components\Button.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine\AssetManager.h"
#include "SlashGameInstance.h"

void ULobbyWidget::NativeOnInitialized()
{
	StartBtn->OnReleased.AddDynamic(this, &ULobbyWidget::StartGame);
	QuitBtn->OnReleased.AddDynamic(this, &ULobbyWidget::EndGame);
}

void ULobbyWidget::StartGame()
{
	FInputModeGameOnly InputMode;

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GetGameInstance<USlashGameInstance>()->GetLevel("SurvivalMap"));
	RemoveFromParent();
}

void ULobbyWidget::EndGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false );
}
