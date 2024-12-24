// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
class USoundBase;
class UBoxComponent;
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	//생성자.
	AWeapon();
	//아이템 장착.
	void Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner, APawn* NewInstigator);
	void UnEquip(AActor* InOwner);
	//Effect종료.
	void DeactivateEmbers();
	//장착 해제시 Effect복원
	void ActivateEmbers();
	//장착한 경우 Collision 제거.
	void DisableSphereCollision();
	//장착 가능하도록 Collision 복원
	void EnableSphereCollision();
	//장착시 소리 재생.
	void PlayEquipSound();
	//소켓을 매개변수로 받아서 해당 소켓에 이 Weapon을 장착.
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	//공격시 중복 방지를 위한 Array.
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;
	//onBoxOverlap은 오버라이드가 아닌 Weapon.h에서 선언된 함수라서 UFUNCTION이 필요함.
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Enmey인지 태그확인.
	bool ActorSameType(AActor* OtherActor);
	//공격에 성공한경우 Execute_GetHit함수 실행.
	void ExecuteGetHit(FHitResult& BoxHit);
	//Breakable Object에 영향을 줄 수 있게 함. 블루프린트에서 구현.
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	//공격 Montage 실행중일때 Overlap검사.
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	//공격 디버깅.
	bool bShowBoxDebug = false;
	//공격 BoxTrace 크기.
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	//장착 사운드.
	UPROPERTY(EditAnywhere,Category="Weapon Properties")
	USoundBase* EquipSound;
	//공격시 활성/비활성으로 공격 구현.
	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	UBoxComponent* WeaponBox;
	//Trace 시작위치.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	//Trace End위치.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	//현재 무기의 데미지.
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
