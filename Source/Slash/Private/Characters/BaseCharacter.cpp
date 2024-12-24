// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
/*boxcomponent*/
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Weapons/Weapon.h"
/*HP*/
#include "Components/AttributeComponent.h"
//hitsounds
#include "Kismet/GameplayStatics.h"
#include "Slash/DebugMacros.h"

ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag("Dead")) {
		CombatTarget = nullptr;
	}
}
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *Hitter->GetActorNameOrLabel());
	if (IsAlive()&&Hitter) DirectionalHitReact(Hitter->GetActorLocation());
	else Die();
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}
int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX) {
		DeathPose = Pose;
	}
	return Selection;
	//return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25,AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget==nullptr) return FVector();
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe=(Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	//DRAW_SPHERE(CombatTargetLocation + TargetToMe);
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}
void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	/*113*/
	const FVector Forward = GetActorForwardVector();
	//Lower Impact Point to the Ememy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	//Forward * ToHit=|Forward||ToHit|*cos(theta)
	//|Forward|=1,|ToHit|=1, so Forward*ToHit=cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	//convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	//CrossProduct Points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) {
		Theta *= -1.f;
	}
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f) {
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f) {
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f) {
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);
	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.0f);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.0f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Red, 5.0f);
	*/
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld()) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage) {
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() < 0)return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes) {
		Attributes->ReceiveDamage(DamageAmount);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->IgnoreActors.Empty();
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}



