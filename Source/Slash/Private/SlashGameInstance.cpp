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
    //위젯 동기적으로 생성.
    const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass.LoadSynchronous(), TEXT("LoadingScreen"));
    TSharedRef<SWidget> LoadingSWidgetPtr = LoadingWidget->TakeWidget();
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    //로딩화면 최소 2초 유지.
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
    //기본 로딩 위젯
    //LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    LoadingScreenAttributes.WidgetLoadingScreen = LoadingSWidgetPtr;
    //로딩 시작.
    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}
void USlashGameInstance::OnDestinatioonWorldLoaded(UWorld* LoadedWorld)
{   //로딩 종료
    GetMoviePlayer()->StopMovie();
}
TSoftObjectPtr<UWorld> USlashGameInstance::GetLevel(FString LevelName)
{
    //맵 가져오기.
    if (MapList.IsEmpty()) {
        UE_LOG(LogTemp, Warning, TEXT("Map List Is Null Please Fill"));
        return nullptr;
    }
    return *MapList.Find(LevelName);
}