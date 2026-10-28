// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerStats;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class FINAL_CORPSE_2_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Primary", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.ActorComponent", MustImplement = "/Script/Final_Corpse_2.Ability"))
	FComponentReference PrimaryAbilityReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Primary")
	bool bIsPrimaryAbilityHoldable;
	
	TScriptInterface<IAbility> ActivePrimaryAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Secondary", meta = (UseComponentPicker, AllowedClasses = "/Script/Engine.ActorComponent", MustImplement = "/Script/Final_Corpse_2.Ability"))
	FComponentReference SecondaryAbilityReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities|Secondary")
	bool bIsSecondaryAbilityHoldable;
	
	TScriptInterface<IAbility> ActiveSecondaryAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	TObjectPtr<UPlayerStats> Stats;

	void Look(const FInputActionValue& Value);
	void DoJump();
	void StopJump();
	void Move(const FInputActionValue& Value);
	
	void PrimaryAbility(const FInputActionValue& Value);
	void SecondaryAbility(const FInputActionValue& Value);
};
