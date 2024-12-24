// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * UMG모듈추가 필요.
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	//HealthBar 설정.
	void SetHealthPercent(float Percent);
private:
	//가비지 데이터로 초기화 되지 않도록 .
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
