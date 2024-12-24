// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//HUD
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
//ai navi
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
//Weapon
#include "Weapons/Weapon.h"
#include "items/Soul.h"

#include "Navigation\PathFollowingComponent.h"

AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//ECollisionChannel은 물체가 충돌하는 것에 대한 채널을 나타내는 열거형.
	//ECollisionResponse는 충돌이 발생하면 어떻게 반응할지 나타내는 열거형.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);//해당 컴포넌트가 다른 오브젝트와 충돌할 때 Overlap이벤트로 감지할 것인지.

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->bOrientRotationToMovement = true;
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
	GetCharacterMovement()->MaxWalkSpeed = 125.f;
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HealthBar(false);
	SpawnDefaultWeapon();
	Tags.Add(FName("Enemy"));
	if (AttackMontage) {
		for (int i = 0; i < AttackMontage->GetNumSections(); i++) {
			AttackMontageSections.Add(AttackMontage->GetSectionName(i));
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (GetWorld()) {
		DrawDebugSphere(GetWorld(), GetActorLocation(), CombatRadius, 12, FColor::Red);
	}
	if (EnemyState > EEnemyState::EES_Patrolling) {
		CheckCombatTarget();
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("movemove"));
		CheckPatrolTarget();
	}
}
void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
	if (AttackMontage) {
		for (int i = 0; i < AttackMontage->GetNumSections(); i++) {
			AttackMontageSections.Add(AttackMontage->GetSectionName(i));
		}
	}
}
void AEnemy::PatrolTImerFinished()
{
	MoveToTarget(PatrolTarget);
	
}
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <=Radius;
}
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	//UE_LOG(LogTemp, Warning, TEXT("Move!"));
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);//목표
	MoveRequest.SetAcceptanceRadius(50.f);//접근범위
	EnemyController->MoveTo(MoveRequest);
	//FNavPathSharedPtr NavPath;
	//EnemyController->MoveTo(MoveRequest, &NavPath);
	//TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
	//for (auto& Point : PathPoints) {
	//	const FVector& Location = Point.Location;
	//	DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
	//}
}
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets) {
		if (Target != PatrolTarget) {
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0) {
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));
	if (bShouldChaseTarget) {
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
		//UE_LOG(LogTemp, Warning, TEXT("Seen Pawn, Now Chasing"));
	}
	
}
bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}
void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}
void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();

}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) {
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin,PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTImerFinished, WaitTime);
	}
}
void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius()) {
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
		
	}
	else if (IsOutsideAttackRadius()&& !IsChasing()) {
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack()) {
		StartAttackTimer();
	}
}
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	
	if(!IsDead())HealthBar(true);
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HandleDamage(DamageAmount);
	
	CombatTarget=EventInstigator->GetPawn();
	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if(IsOutsideAttackRadius()){
		ChaseTarget();
	}
	
	return DamageAmount;
}
void AEnemy::Die()
{
	Super::Die();
	EnemyState = EEnemyState::EES_Dead;
	
	ClearAttackTimer();
	HealthBar(false);
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	SpawnSoul();
}
void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass&&Attributes) {
		//const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 75.f);
		ASoul* SpawnSoul =World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnSoul) {
			SpawnSoul->SetSouls(Attributes->GetSoul());
		}
	}
}
void AEnemy::Destroyed()
{
	if (EquippedWeapon) {
		EquippedWeapon->Destroy();
	}
}
void AEnemy::HealthBar(bool visible)
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(visible);
	}
}
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HealthBar(false);
}
void AEnemy::StartPatrolling()
{
	//UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}
void AEnemy::ChaseTarget()
{
	//outside Attack Range, chase Character
	//UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}
bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsChasing()
{
	return  EnemyState == EEnemyState::EES_Chasing;
}
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}
bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}
bool AEnemy::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}
void AEnemy::StartAttackTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack"));
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTIme = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack,AttackTIme);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}
