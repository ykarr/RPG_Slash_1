// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterType.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
	
	
public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	/*Combat*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	virtual void Attack();
	virtual bool CanAttack();
	virtual bool IsAlive();
	virtual void HandleDamage(float DamageAmount);
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	void DisableMeshCollision();
	/*Montage*/
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere,Category=Combat)
	double WarpTargetDistance = 100.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName);

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;
public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
