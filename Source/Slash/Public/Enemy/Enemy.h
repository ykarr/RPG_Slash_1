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
	//Enemy의 공격이 끝났을 때 호출.
	virtual void AttackEnd() override;
	/*</ABaseCharacter>*/
	/*blueprint때문에 private*/
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:
	/*AI_Behavior*/
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTImerFinished();
	//HealthBar On/Off
	void HealthBar(bool visible);
	//어그로 풀림.
	void LoseInterest();
	//순찰시작.
	void StartPatrolling();
	//추격.
	void ChaseTarget();
	//전투범위 밖에 있는경우.
	bool IsOutsideCombatRadius();
	//공격반경 밖에 있는지.
	bool IsOutsideAttackRadius();
	//공격반경 안에있는지
	bool IsInsideAttackRadius();
	//쫒는중인지
	bool IsChasing();
	//공격중인지.
	bool IsAttacking();
	bool IsEngaged();
	//죽어있는지
	bool IsDead();
	//살아있는지.
	bool IsAlive();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	//Target으로 이동.
	void MoveToTarget(AActor* Target);
	//PatrolTarget설정.
	AActor* ChoosePatrolTarget();
	//기본 장착 무기 설정.
	void SpawnDefaultWeapon();
	//callback for onPawnSeen in UPawnSensing
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	//HP위젯
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	//Enemy감각.
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	//기본 무기 클래스.
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
	//몬스터를 죽이면 나오는 소울.
	UPROPERTY(EditAnywhere,Category=Combat)
	TSubclassOf<class ASoul> SoulClass;
};
