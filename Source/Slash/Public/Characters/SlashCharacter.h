// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"
/*�Է�*/

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
	//������.
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	//Input�Լ� ����.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//�������� �������. 
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	/*<IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	/*</IHitInterface>*/
	//Collision�� Character�� ��Ҵ���.
	virtual void SetOverlappingItem(class AItem* Item)override;
	//Enmey�� ���̸� ������ Soul�� ����.
	virtual void AddSouls(class ASoul* Soul) override;
	//Obejct���� ������ Gold�� ����.
	virtual void AddGold(class ATreasure* Treasure) override;
protected:
	//���� �����ϸ� �����.
	virtual void BeginPlay() override;
	/*Callback Input*/
	//ĳ���� ������.
	void Move(const FInputActionValue& value);
	//ī�޶� ������.
	void Look(const FInputActionValue& value);
	//����.
	void Jump() override;
	//������ ���� �Լ�.
	void EKeyPressed();
	//������ ���� ���� �Լ�.
	void GKeyPressed();
	//������.
	void Dodge();
	//���� ����� ���¹̳ʰ� �ִ���.
	bool HasEnoughStamina();
	//���� ���� �ϰ� �ִ��� Ȯ���ϴ� �Լ�.
	bool IsOccupied();
	//����.
	virtual void Attack() override;
	//�޴� ����.
	void OpenMenu();
	
	/*Combat*/
	//������ ����.
	void EquipWeapon(AWeapon* Weapon);
	//������ �� �ִ���.
	virtual bool CanAttack() override;
	//���� Montage����.
	void PlayEquipMontage(const FName& SectionName);
	//������ �� �ִ��� ĳ���� ����Ȯ��.
	bool CanArm();
	//���� ������ �� �ִ��� ĳ���� ����Ȯ��.
	bool CanDisarm();
	//���� ����.
	void Disarm();
	//����.
	void Arm();
	//�׾�����.
	virtual void Die() override;
	//���� ����. ĳ���� ���� ����.
	virtual void AttackEnd()override;
	//������ ��. ĳ���� ���� ����.
	virtual void DodgeEnd()override;
	//� ���� ���̱�.(Anim Notify)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	//�տ� ���� ���̱�.(Anim Notify)
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	//���� ���� ��.
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	//�´� ������ ����.
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	//InputContext
	UPROPERTY(EditAnywhere,Category=Input)
	UInputMappingContext* SlashContext;
	//ĳ���� �����ӿ� ���õ� inputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;
	//�÷��̾ ���� ȭ�鿡 ���� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	//�÷��̾� ĳ������ Jump�� ���� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	//���� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
	//�÷��̾� ������ InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
	//�ٴڿ� �ִ� ���⸦ �ֿ� �� ����ϴ� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;
	//�ΰ��ӿ��� Menu�� �ҷ����� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MenuAction;
	//���� ����ִ� ���⸦ �������� InputAction
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DropAction;
	
	//���� �߰��� �޴� ������ �ҷ����� ���� Ŭ���� (���� �ε�) 
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> MenuWidgetClass;
	//���ӿ��� �׾����� ������ ������ ���� Ŭ���� (���� �ε�)
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> PlayerLostClass;
	//Menu On/Off�� ���� ����.
	UUserWidget* MenuWidget;
private:
	//UI �ʱ�ȭ.
	void InitializeSlashOverlay();
	//UI Health����
	void SetHUDHealth();
	//���� �ƹ��͵� ���ϴ� �������� Ȯ��.
	bool IsUnoccupied();
	//ĳ���� �������� ����. �ʱⰪ ��������.
	ECharacterState CharacterState=ECharacterState::ECS_Unequipped;
	//ĳ���� �ൿ ����. �ʱⰪ ��������.
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/*Character Components*/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	//ī�޶� ������Ʈ.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	//�Ӹ�ī��. �÷����� �ʿ�.(�������� Default�� true�� �ٲ㼭 �����.)
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
	//��� ���� �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;
	//��ġ�� ������(������ �ݱ�).
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	//UI
	UPROPERTY()
	USlashOverlay* SlashOverlay;
public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
