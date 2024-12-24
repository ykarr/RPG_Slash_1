// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Item.h"
#include "Slash/DebugMacros.h" //custom headerfile
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
/*pickup Sound*/
#include "Kismet/GameplayStatics.h"
// Sets default values
AItem::AItem():Amplitude(0.25f), TimeConstant(5.f) //초기화.
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Amplitude = 0.25f; // Amplitude 초기화 
	ItemMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());//GetRootComponent();
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	//int32 AvgInt=Avg<int32>(1, 3);
	//UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %d"), AvgInt);
	/*UE_LOG(LogTemp, Warning, TEXT("Begin Play called"));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Cyan,FString("hello world"));
	}*/
	/*UWorld* world = GetWorld();
	if (world) {
		FVector Location = GetActorLocation();
		DrawDebugSphere(world, Location,25.f, THIRTY,FColor::Cyan,false,30.f);
	}*/
	
	//SetActorLocation(FVector(0.f, 0.f, 50.f));
	//SetActorRotation(FRotator(0.f,45.f,0.f));
	//UWorld* world = GetWorld();
	
	//FVector Location = GetActorLocation();
	
	//DRAW_LINE(Location, Location +GetActorForwardVector()*100.f);
	//DRAW_POINT(Location+GetActorForwardVector() * 100.f);
	//DRAW_VECTOR(Location, Location + GetActorForwardVector() * 100.f)
	//DRAW_SPHERE(Location);
	
}

float AItem::TransformSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformCosin()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//const FString OtherActorName=OtherActor->GetName();
	//if (GEngine) {
	//	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	//}
	IPickupInterface* HitInterface = Cast<IPickupInterface>(OtherActor);
	if (HitInterface) {
		HitInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (GEngine) {
	//	GEngine->AddOnScreenDebugMessage(2,30.f,FColor::Red,FString(TEXT("End OverLap")));
	//}
	IPickupInterface* HitInterface = Cast<IPickupInterface>(OtherActor);
	if (HitInterface) {
		HitInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound) {
		UGameplayStatics::SpawnSoundAtLocation(this,PickupSound,GetActorLocation());
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 1.0f) {
		//Menu를 사용해서 멈춘 상태가 아니고 아이템이 Hovering상태인 경우.
		if (ItemState == EItemState::EIS_Hovering) {
			AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()));
		}
		RunningTime += DeltaTime;
	}
	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);
	/*if (GEngine) {
		FString Name = GetName(); //*Name으로 사용.
		FString Message = FString::Printf(TEXT("DeltaTime: %s"), *Name);
		//FString Message = FString::Printf(TEXT("DeltaTime: %f"), DeltaTime);
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Cyan, Message);
	}*/
	
	//이런식으로 코드를 작성해 50cm/s
	//float MovementRate = 50.f;
	//float RotationRate = 45.f;
	//AddActorWorldOffset(FVector(MovementRate*DeltaTime, 0.f, 0.f));
	//AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

	
	//GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Cyan, FString::Printf(TEXT("RunningTime: %f %f"), RunningTime,DeltaTime));
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f)
}

