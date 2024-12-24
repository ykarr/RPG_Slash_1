// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
ABird::ABird()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);

	//RootComponent = Capsule;
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());//RootComponent, Capsule;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
			Subsystem->AddMappingContext(BirdMappingContext,0);
		}
	}
}

void ABird::MoveForward(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Value: %f"), value);
}

void ABird::Move(const FInputActionValue& value)
{
	const float DirectionValue=value.Get<float>();
	if (Controller&&(DirectionValue!=0.f)) {
		
		FVector Forward = GetActorForwardVector();
		UE_LOG(LogTemp, Warning, TEXT("IA_Move triggered"));
		AddMovementInput(Forward, DirectionValue);
	}
}



void ABird::LookUp(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::LookUp);
		
	}
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
}

