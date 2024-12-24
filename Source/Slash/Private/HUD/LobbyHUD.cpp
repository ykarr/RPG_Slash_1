// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyHUD.h"
#include "HUD/UI/LobbyWidget.h"
ALobbyHUD::ALobbyHUD()
{
	//맨 뒤에는 _C를 붙여 C로부터 만들어진 것임을 명시.
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
			//LobbyWidgetClass를 성공적으로 가지고 있고 PlayerController도 유효하다면 위젯을 생성.
			LobbyWidget = CreateWidget<ULobbyWidget>(PlayerController, LobbyWidgetClass);
			LobbyWidget->AddToViewport(0);
			//마우스 커서 보이고 UI모드로 설정.
			PlayerController->SetShowMouseCursor(1);
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
	}
}
