// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/MenuWidget.h"

#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"

#include "SlashGameInstance.h"
void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BackGameBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::BacktoGame);
	MenuBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::BacktoMenu);
	QuitBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::QuitGame);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//#include "Animation/WidgetAnimation.h"
	PlayAnimation(MenuAnim);
	if (GetOwningPlayer()) {
	FInputModeGameAndUI InputMode;
	//FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
	}
	//#include "Kismet\GameplayStatics.h"
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
}

void UMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (GetOwningPlayer()) {
		
		FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void UMenuWidget::BacktoGame()
{
	RemoveFromParent();
}

void UMenuWidget::BacktoMenu()
{
	FInputModeUIOnly InputMode;

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GetGameInstance<USlashGameInstance>()->GetLevel("MainMenu"));

	RemoveFromParent();
}

void UMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit,true);
}
