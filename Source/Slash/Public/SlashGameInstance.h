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
	//게임 인스턴스가 처음 생성될 때 호출됨.
	virtual void Init()override;
	UFUNCTION()//맵 로드가 시작되기 전에 로딩화면을 설정.
	void OnPreLoadMap(const FString& PreLoadMap);
	UFUNCTION()//맵이 로드된 후 로딩 화면을 종료.
	void OnDestinatioonWorldLoaded(UWorld* LoadedWorld);
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> LoadingWidgetClass;

private:
	//String을 키로 사용해서 저장된 Level을 가져옴
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TMap<FString, TSoftObjectPtr<UWorld>> MapList;
};
