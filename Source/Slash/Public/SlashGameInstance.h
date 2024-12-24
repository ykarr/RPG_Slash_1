// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlashGameInstance.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class SLASH_API USlashGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	TSoftObjectPtr<UWorld> GetLevel(FString LevelName);
protected:
	//���� �ν��Ͻ��� ó�� ������ �� ȣ���.
	virtual void Init()override;
	UFUNCTION()//�� �ε尡 ���۵Ǳ� ���� �ε�ȭ���� ����.
	void OnPreLoadMap(const FString& PreLoadMap);
	UFUNCTION()//���� �ε�� �� �ε� ȭ���� ����.
	void OnDestinatioonWorldLoaded(UWorld* LoadedWorld);
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> LoadingWidgetClass;

private:
	//String�� Ű�� ����ؼ� ����� Level�� ������
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TMap<FString, TSoftObjectPtr<UWorld>> MapList;
};
