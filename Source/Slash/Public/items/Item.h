// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"
class USphereComponent;
class UNiagaraComponent;
class USoundBase;
enum class EItemState :uint8 {
	EIS_Hovering,
	EIS_Equipped
};
UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//UPROPERTY(EditDefaultsOnly)
	UPROPERTY(EditAnyWhere,BlueprintReadOnly, Category = "Sine Parameters")
	float Amplitude;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="Sine Parameters")
	float TimeConstant;

	//UFUNCTION(BlueprintCallable)
	UFUNCTION(BlueprintPure)
	float TransformSin();
	UFUNCTION(BlueprintPure)
	float TransformCosin();
	template<typename T>
	T Avg(T first,T second);
	/*PrimitiveComponent.h*/
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState=EItemState::EIS_Hovering;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;
	UPROPERTY(EditAnywhere,Category=Sound)
	USoundBase* PickupSound;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	float RunningTime;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;
	
	//UPROPERTY(EditInstanceOnly)
	
};

template<typename T>
inline T AItem::Avg(T first, T second)
{
	return (first+second)/2;
}
