// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyHUD.h"
#include "HUD/UI/LobbyWidget.h"
ALobbyHUD::ALobbyHUD()
{
	//�� �ڿ��� _C�� �ٿ� C�κ��� ������� ������ ���.
	static ConstructorHelpers::FClassFinder<ULobbyWidget> FindLobbyClass(TEXT("/Game/Blueprints/HUD/WBP_LobbyWidget.WBP_LobbyWidget_C"));
	if (FindLobbyClass.Succeeded()) {
		LobbyWidgetClass = FindLobbyClass.Class;
	} 
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()) {
		TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
		checkf(LobbyWidgetClass, TEXT("LobbyWidgetClass is Empty or Error, please Fill LobbyHUD!"));
		if (IsValid(PlayerController)) {
			//LobbyWidgetClass�� ���������� ������ �ְ� PlayerController�� ��ȿ�ϴٸ� ������ ����.
			LobbyWidget = CreateWidget<ULobbyWidget>(PlayerController, LobbyWidgetClass);
			LobbyWidget->AddToViewport(0);
			//���콺 Ŀ�� ���̰� UI���� ����.
			PlayerController->SetShowMouseCursor(1);
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
	}
}
