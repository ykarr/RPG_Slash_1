// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"

void USlashGameInstance::Init()
{
    Super::Init();
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinatioonWorldLoaded);
}

void USlashGameInstance::OnPreLoadMap(const FString& PreLoadMap)
{
    //���� ���������� ����.
    const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass.LoadSynchronous(), TEXT("LoadingScreen"));
    TSharedRef<SWidget> LoadingSWidgetPtr = LoadingWidget->TakeWidget();
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    //�ε�ȭ�� �ּ� 2�� ����.
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
    //�⺻ �ε� ����
    //LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    LoadingScreenAttributes.WidgetLoadingScreen = LoadingSWidgetPtr;
    //�ε� ����.
    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}
void USlashGameInstance::OnDestinatioonWorldLoaded(UWorld* LoadedWorld)
{   //�ε� ����
    GetMoviePlayer()->StopMovie();
}
TSoftObjectPtr<UWorld> USlashGameInstance::GetLevel(FString LevelName)
{
    //�� ��������.
    if (MapList.IsEmpty()) {
        UE_LOG(LogTemp, Warning, TEXT("Map List Is Null Please Fill"));
        return nullptr;
    }
    return *MapList.Find(LevelName);
}