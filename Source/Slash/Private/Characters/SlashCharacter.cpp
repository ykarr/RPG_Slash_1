// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
/*입력*/
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
/*camera*/
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
/*Rotate*/
#include "GameFramework/CharacterMovementComponent.h"
/*hair*/
#include "GroomComponent.h"
/*Item*/
#include "items/Item.h"
#include "Weapons/Weapon.h"
/*Anim*/
#include "Animation/AnimMontage.h"
/*gethit*/
#include "Components/StaticMeshComponent.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Components/AttributeComponent.h"
#include "items/Soul.h"
#include "items/Treasure.h"

#include "Blueprint/UserWidget.h"
//#include "Engine\AssetManager.h"
#include "Kismet\GameplayStatics.h"
// Sets default values
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
	Tags.Add(FName("EngageableTarget"));
	InitializeSlashOverlay();
	if (AttackMontage) {
		for (int i = 0; i < AttackMontage->GetNumSections(); i++) {
			AttackMontageSections.Add(AttackMontage->GetSectionName(i));
		}
	}
}
void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD) {
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes) {
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSoul(0);
			}
		}
	}
}
void ASlashCharacter::Move(const FInputActionValue& value)
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 0.f) {
		return;
	}
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
	//if(GetController())
}

void ASlashCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Jump()
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 0.f) {
		return;
	}
	if (IsUnoccupied()) {
		Super::Jump();
	}
}
bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}
void ASlashCharacter::EKeyPressed()
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 0.f) {
		return;
	}
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) {
		EquipWeapon(OverlappingWeapon);
	}
	else {
		if (CanDisarm()) Disarm();
		else if (CanArm()) Arm();
	}
}
void ASlashCharacter::GKeyPressed()
{
	if (EquippedWeapon) {
		EquippedWeapon->UnEquip(this);
		EquippedWeapon = nullptr;
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_Unoccupied;
	}
}
void ASlashCharacter::Dodge()
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 0.f) {
		return;
	}
	if (IsOccupied()||!HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes&&SlashOverlay) {
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}
bool ASlashCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() >Attributes->GetDodgeCost();
}
bool ASlashCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}
void ASlashCharacter::Attack()
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) == 0.f) {
		return;
	}
	Super::Attack();
	if (CanAttack()) {
	
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::OpenMenu()
{
	UE_LOG(LogTemp, Display, TEXT("menu"));
	//메뉴가 열려있다면 닫기.
	if (IsValid( MenuWidget)) {
		MenuWidget->RemoveFromParent();
		MenuWidget = nullptr;
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); // 시간 흐름 복원
		return;
	}
	// 메뉴가 닫혀 있다면 열기
	if (!MenuWidget && !MenuWidgetClass.IsNull())
	{
		// 위젯 클래스 동기 로드
		TSubclassOf<UUserWidget> GetMenuWidgetClass = MenuWidgetClass.LoadSynchronous();
		if (GetMenuWidgetClass)
		{
			MenuWidget = CreateWidget<UUserWidget>(GetWorld(), GetMenuWidgetClass);
			if (MenuWidget)
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f); // 시간 정지
				MenuWidget->AddToViewport(5);
			}
		}
	}
	
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (!EquippedWeapon) {
		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = Weapon;
	}
	
}



void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}
bool ASlashCharacter::CanAttack()
{
	//공격가능한 상태인지 무기가 있는지.
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}
void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance &&EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}
bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped&& EquippedWeapon;
}
bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped ;
}
void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}
void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}
void ASlashCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
	TSubclassOf<UUserWidget>GetPlayerLostClass= PlayerLostClass.LoadSynchronous();
	UUserWidget* PlayerLost= CreateWidget<UUserWidget>(GetWorld(), GetPlayerLostClass);
	PlayerLost->AddToViewport(10);
}
void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(),FName("SpineSocket"));
	}
}
void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}
void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attributes) {
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}
// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ASlashCharacter::OpenMenu);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ASlashCharacter::GKeyPressed);
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HandleDamage(DamageAmount);
	SetHUDHealth();
	
	return DamageAmount;
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes) {
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	UE_LOG(LogTemp, Display, TEXT("GetHit_ImplementationGetHit_ImplementationGetHit_Implementation"));
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes&&SlashOverlay) {
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSoul(Attributes->GetSoul());
	}
}

void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes&& SlashOverlay) {
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}


