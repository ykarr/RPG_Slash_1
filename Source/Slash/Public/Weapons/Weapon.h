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
	//������.
	AWeapon();
	//������ ����.
	void Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner, APawn* NewInstigator);
	void UnEquip(AActor* InOwner);
	//Effect����.
	void DeactivateEmbers();
	//���� ������ Effect����
	void ActivateEmbers();
	//������ ��� Collision ����.
	void DisableSphereCollision();
	//���� �����ϵ��� Collision ����
	void EnableSphereCollision();
	//������ �Ҹ� ���.
	void PlayEquipSound();
	//������ �Ű������� �޾Ƽ� �ش� ���Ͽ� �� Weapon�� ����.
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	//���ݽ� �ߺ� ������ ���� Array.
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;
	//onBoxOverlap�� �������̵尡 �ƴ� Weapon.h���� ����� �Լ��� UFUNCTION�� �ʿ���.
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Enmey���� �±�Ȯ��.
	bool ActorSameType(AActor* OtherActor);
	//���ݿ� �����Ѱ�� Execute_GetHit�Լ� ����.
	void ExecuteGetHit(FHitResult& BoxHit);
	//Breakable Object�� ������ �� �� �ְ� ��. �������Ʈ���� ����.
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	//���� Montage �������϶� Overlap�˻�.
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	//���� �����.
	bool bShowBoxDebug = false;
	//���� BoxTrace ũ��.
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	//���� ����.
	UPROPERTY(EditAnywhere,Category="Weapon Properties")
	USoundBase* EquipSound;
	//���ݽ� Ȱ��/��Ȱ������ ���� ����.
	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	UBoxComponent* WeaponBox;
	//Trace ������ġ.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	//Trace End��ġ.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	//���� ������ ������.
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
