// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * UMG����߰� �ʿ�.
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	//HealthBar ����.
	void SetHealthPercent(float Percent);
private:
	//������ �����ͷ� �ʱ�ȭ ���� �ʵ��� .
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
