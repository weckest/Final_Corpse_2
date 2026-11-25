// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Character/BaseCharacter.h"
#include "Data/GameplayEventData.h"
#include "PlayerCharacter.generated.h"

class UGameOverWidget;
class UPlayerStats;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayEvent, const FGameplayEventData&, GameplayEventData);
UCLASS()
class FINAL_CORPSE_2_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameplayEvent GamePlayEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Landed(const FHitResult& Hit) override;
	
	float LastCrouchPressTime = -1.0f;
	bool bIsFastFalling = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> PrimaryAbilityAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> SecondaryAbilityAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> SpecialAbilityAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> GroundDashAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> AirDashAction;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsDashing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DashLockoutTime = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundDashVelocity = 2750.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AirDashVelocity = 750.0f;
	
	FTimerHandle DashLockoutTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Primary")
	bool bIsPrimaryAbilityHoldable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Primary", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.ActorComponent", MustImplement = "/Script/Final_Corpse_2.Ability"))
	FComponentReference PrimaryAbilityReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Primary")
	TObjectPtr<UAbility> ActivePrimaryAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Secondary")
	bool bIsSecondaryAbilityHoldable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Secondary", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.ActorComponent", MustImplement = "/Script/Final_Corpse_2.Ability"))
	FComponentReference SecondaryAbilityReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Secondary")
	TObjectPtr<UAbility> ActiveSecondaryAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Passive", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.ActorComponent"))
	TArray<FComponentReference> PassiveAbilityReference;
	
	TArray<TScriptInterface<UActorComponent>> ActivePassiveAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	TObjectPtr<UPlayerStats> Stats;

	void Look(const FInputActionValue& Value);
	void DoJump();
	void StopJump();
	void Move(const FInputActionValue& Value);
	
	void OnCrouchPressed();
	void OnCrouchReleased();
	void TriggerFastFall();
	
	void PrimaryAbility(const FInputActionValue& Value);
	void SecondaryAbility(const FInputActionValue& Value);
	
	void GroundDash(const FInputActionValue& Value);
	void AirDash(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	float WallCheckDistance = 65.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	float WallJumpHorizontalForce = 750.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	float WallJumpVerticalForce = 600.0f;
	
	UFUNCTION(BlueprintCallable, Category = "WallJump")
	bool TryWallJump();
	
	bool FindWall(FHitResult& OutHit) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastFall")
	float DoubleTapWindow = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastFall")
	float FastFallSpeed = 1500.0f;
	
	UFUNCTION()
	void Hit(const FGameplayEventData& EventData);
	
	bool bIsDead = false;
	
	FTimerHandle DeathTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void ShowGameOverUI();
	
};
