// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
//#include "AIController.h"
#include "Enemy.generated.h"

//class UAnimMontage;
class UHealthBarComponent;
class UPawnSensingComponent;
UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	/*<AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void Destroyed() override;
	/*</AActor>*/
	/*<IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/*</IHitInterface>*/
protected:
	/*<AActor>*/
	virtual void BeginPlay() override;
	/*</AActor>*/
	/*<ABaseCharacter>*/
	virtual void Die() override;
	void SpawnSoul();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	//Enemy�� ������ ������ �� ȣ��.
	virtual void AttackEnd() override;
	/*</ABaseCharacter>*/
	/*blueprint������ private*/
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:
	/*AI_Behavior*/
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTImerFinished();
	//HealthBar On/Off
	void HealthBar(bool visible);
	//��׷� Ǯ��.
	void LoseInterest();
	//��������.
	void StartPatrolling();
	//�߰�.
	void ChaseTarget();
	//�������� �ۿ� �ִ°��.
	bool IsOutsideCombatRadius();
	//���ݹݰ� �ۿ� �ִ���.
	bool IsOutsideAttackRadius();
	//���ݹݰ� �ȿ��ִ���
	bool IsInsideAttackRadius();
	//�i��������
	bool IsChasing();
	//����������.
	bool IsAttacking();
	bool IsEngaged();
	//�׾��ִ���
	bool IsDead();
	//����ִ���.
	bool IsAlive();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	//Target���� �̵�.
	void MoveToTarget(AActor* Target);
	//PatrolTarget����.
	AActor* ChoosePatrolTarget();
	//�⺻ ���� ���� ����.
	void SpawnDefaultWeapon();
	//callback for onPawnSeen in UPawnSensing
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	//HP����
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	//Enemy����.
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	//�⺻ ���� Ŭ����.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;
	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float PatrolWaitMin=5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;
	
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.3f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 0.6f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;
	//���͸� ���̸� ������ �ҿ�.
	UPROPERTY(EditAnywhere,Category=Combat)
	TSubclassOf<class ASoul> SoulClass;
};
