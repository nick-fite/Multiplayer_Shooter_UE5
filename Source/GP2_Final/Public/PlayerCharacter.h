// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCrosshair;
class AWeapon;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GP2_FINAL_API APlayerCharacter : public ACharacter
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

private:
	UFUNCTION() void Move(const FInputActionValue& InputValue);
	UFUNCTION() void Look(const FInputActionValue& InputValue);

	UFUNCTION() FVector GetMoveForwardDir() const;
	UFUNCTION() FVector GetMoveRightDir() const;
	
	//camera
	UPROPERTY(VisibleAnywhere, Category="View") USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category="View") UCameraComponent* ViewCamera;
	UPROPERTY(EditAnywhere, Category="View") FVector SpringArmPosition;	

	UPROPERTY(EditAnywhere, Category = "Input") UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, category="Input") UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, category="Input") UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, category="Input") UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input") float LookSensitivity {0.5f};

	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<AWeapon> WeaponToSpawn;
	UPROPERTY(EditAnywhere, Category="Weapon") AWeapon* SpawnedWeapon;


	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<UCrosshair> CrosshairToSpawn;
	UPROPERTY(EditAnywhere, Category="Weapon") UCrosshair* SpawnedCrosshair;
	
};
