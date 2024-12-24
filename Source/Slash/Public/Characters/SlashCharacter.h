// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"
/*입력*/

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class USlashOverlay;
class UUserWidget;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter,public IPickupInterface
{
	GENERATED_BODY() 

public:
	//생성자.
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	//Input함수 설정.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//데미지를 받은경우. 
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	/*<IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	/*</IHitInterface>*/
	//Collision에 Character가 닿았는지.
	virtual void SetOverlappingItem(class AItem* Item)override;
	//Enmey를 죽이면 나오는 Soul을 얻음.
	virtual void AddSouls(class ASoul* Soul) override;
	//Obejct에서 나오는 Gold를 얻음.
	virtual void AddGold(class ATreasure* Treasure) override;
protected:
	//게임 시작하면 실행됨.
	virtual void BeginPlay() override;
	/*Callback Input*/
	//캐릭터 움직임.
	void Move(const FInputActionValue& value);
	//카메라 움직임.
	void Look(const FInputActionValue& value);
	//점프.
	void Jump() override;
	//아이템 장착 함수.
	void EKeyPressed();
	//아이템 장착 해제 함수.
	void GKeyPressed();
	//구르기.
	void Dodge();
	//현재 충분한 스태미너가 있는지.
	bool HasEnoughStamina();
	//현재 무언가 하고 있는지 확인하는 함수.
	bool IsOccupied();
	//공격.
	virtual void Attack() override;
	//메뉴 열기.
	void OpenMenu();
	
	/*Combat*/
	//아이템 장착.
	void EquipWeapon(AWeapon* Weapon);
	//공격할 수 있는지.
	virtual bool CanAttack() override;
	//장착 Montage실행.
	void PlayEquipMontage(const FName& SectionName);
	//무장할 수 있는지 캐릭터 상태확인.
	bool CanArm();
	//무장 해제할 수 있는지 캐릭터 상태확인.
	bool CanDisarm();
	//무장 해제.
	void Disarm();
	//무장.
	void Arm();
	//죽었을때.
	virtual void Die() override;
	//공격 종료. 캐릭터 상태 복구.
	virtual void AttackEnd()override;
	//구르기 끝. 캐릭터 상태 복구.
	virtual void DodgeEnd()override;
	//등에 무기 붙이기.(Anim Notify)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	//손에 무기 붙이기.(Anim Notify)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	//무기 장착 끝.
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	//맞는 리엑션 종료.
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	//InputContext
	UPROPERTY(EditAnywhere,Category=Input)
	UInputMappingContext* SlashContext;
	//캐릭터 움직임에 관련된 inputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;
	//플레이어가 보는 화면에 대한 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	//플레이어 캐릭터의 Jump에 대한 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	//공격 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
	//플레이어 구르기 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
	//바닥에 있는 무기를 주울 때 사용하는 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;
	//인게임에서 Menu를 불러오는 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MenuAction;
	//현재 들고있는 무기를 내려놓는 InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DropAction;
	
	//게임 중간에 메뉴 위젯을 불러오기 위한 클래스 (동기 로드) 
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> MenuWidgetClass;
	//게임에서 죽었을때 나오는 위젯을 위한 클래스 (동기 로드)
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> PlayerLostClass;
	//Menu On/Off를 위해 선언.
	UUserWidget* MenuWidget;
private:
	//UI 초기화.
	void InitializeSlashOverlay();
	//UI Health설정
	void SetHUDHealth();
	//현재 아무것도 안하는 상태인지 확인.
	bool IsUnoccupied();
	//캐릭터 무기장착 상태. 초기값 점유없음.
	ECharacterState CharacterState=ECharacterState::ECS_Unequipped;
	//캐릭터 행동 상태. 초기값 점유없음.
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/*Character Components*/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	//카메라 컴포넌트.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	//머리카락. 플러그인 필요.(엔진에서 Default값 true로 바꿔서 사용함.)
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
	//장비 장착 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	//겹치는 아이템(아이템 줍기).
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	//UI
	UPROPERTY()
	USlashOverlay* SlashOverlay;
public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
