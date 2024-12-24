// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
//#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
/*BoxTrace*/
#include "Kismet/KismetSystemLibrary.h"
/*interface*/
#include "Interfaces/HitInterface.h"
/*Niagara*/
#include "NiagaraComponent.h"
AWeapon::AWeapon() {
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	//WeaponBox->SetBoxExtent(FVector(2.0f,1.0f,40.f));
	WeaponBox->SetupAttachment(GetRootComponent());
	//ECollisionEnabled::QueryOnly = 충돌검사를 활성화하고, 검사는 진행하되, 충돌 이벤트를 발생시키지는 않음.
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//충돌을 알려주는데 차단하거나, 무시하지는 않음.
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//pawn은 무시.

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}
void AWeapon::Equip(USceneComponent* InParent,FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbers();
}
void AWeapon::UnEquip(AActor* InOwner)
{
	ActivateEmbers();
	PlayEquipSound();
	EnableSphereCollision();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	this->SetActorRotation(FRotator(0, 0, 0));
	this->SetActorLocation(InOwner->GetActorLocation());
}
void AWeapon::DeactivateEmbers()
{
	if (ItemEffect) {
		ItemEffect->Deactivate();
	}
}
void AWeapon::ActivateEmbers()
{
	if (ItemEffect) {
		ItemEffect->Activate();
	}
}
void AWeapon::DisableSphereCollision()
{
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void AWeapon::EnableSphereCollision()
{
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
void AWeapon::PlayEquipSound()
{
	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorSameType(OtherActor)) return;
	if (GetOwner()->ActorHasTag(TEXT("EngageableTarget")) && OtherActor->ActorHasTag(TEXT("EngageableTarget"))) return;
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	if (BoxHit.GetActor()) { //actor가 아닌것을 때릴경우 null이 반환되므로.
		
		if (ActorSameType(BoxHit.GetActor())) return;
		//UE_LOG(LogTemp, Display, TEXT("OnBoxOverlapOnBoxOverlapOnBoxOverlap"));
		//instigator = 게임에서 이벤트를 시작한 플레이어나 캐릭터. 공격을 시작한 플레이어.
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);//applyDamage다음 GetHit에서 피검사?
		CreateFields(BoxHit.ImpactPoint);
		
	}
}

bool AWeapon::ActorSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface) {
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.AddUnique(GetOwner());
	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, BoxTraceExtent, BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1,
		false, ActorsToIgnore, bShowBoxDebug?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None, BoxHit, true);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
