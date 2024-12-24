// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;
class UGeometryCollectionComponent;
UCLASS()
class SLASH_API ABreakableActor : public AActor,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	//������.
	ABreakableActor();
	//���� �������(���ݰ���.).
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//GeometryCollection
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UCapsuleComponent* Capsule;
private:
	//����ǰ ��ϵ�.
	UPROPERTY(EditAnywhere, Category="Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;
	//���� �μ��� ��������.
	bool bBroken = false;
};
