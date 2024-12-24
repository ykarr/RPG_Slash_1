// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class ULobbyWidget;
/**
 * 
 */
UCLASS()
class SLASH_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
public:
	ALobbyHUD();
	virtual void BeginPlay() override;
	TSubclassOf< ULobbyWidget> LobbyWidgetClass;
	TObjectPtr<ULobbyWidget> LobbyWidget;
};
