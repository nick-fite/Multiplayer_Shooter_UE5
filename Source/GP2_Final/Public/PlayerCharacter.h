// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.generated.h"

class UDamageComponent;
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
	UFUNCTION(Server, Reliable) void MoveRPC(const FVector DestLocation);
	UFUNCTION() void Look(const FInputActionValue& InputValue);
	UFUNCTION(Server,Reliable) void LookRPC(const FRotator newRot);

	UFUNCTION() FVector GetMoveForwardDir() const;
	UFUNCTION() FVector GetMoveRightDir() const;

	UFUNCTION() void ADS(const FInputActionValue& InputValue);
	UFUNCTION() void Shoot(const FInputActionValue& InputValue);
	UFUNCTION() void Reload(const FInputActionValue& inputValue);
	UFUNCTION() void Sprint(const FInputActionValue& InputValue);

	//camera
	UPROPERTY(VisibleAnywhere, Category="View") USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category="View") UCameraComponent* ViewCamera;
	UPROPERTY(EditAnywhere, Category="View") FVector SpringArmPosition;	

	UPROPERTY(EditAnywhere, Replicated, Category = "Input") UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, Replicated, category="Input") UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Replicated, Category="Input") UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, Replicated, category="Input") UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Replicated, Category="Input") bool bIsSprinting;
	
	UPROPERTY(EditAnywhere, Replicated, category="Input") UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Replicated, Category="Input") UInputAction* ADSAction;
	UPROPERTY(EditAnywhere, Replicated, Category="Input") UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, Replicated, Category="Input") UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Replicated, Category = "Input") float LookSensitivity {0.5f};

	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<AWeapon> WeaponToSpawn;

	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<UCrosshair> CrosshairToSpawn;
	UPROPERTY(EditAnywhere, Replicated, Category="Weapon") UCrosshair* SpawnedCrosshair;

	UPROPERTY(EditAnywhere, Replicated, Category="Animations") UAnimMontage* PlayerPistolShoot;
	UPROPERTY(EditAnywhere, Replicated, Category="animations") UAnimMontage* PistolShoot;

	UPROPERTY(EditAnywhere, Replicated, Category="Vals") FRotator CapsuleRot;
	UPROPERTY(EditAnywhere, Replicated, Category="Vals") FVector CapsuleLoc;
public:

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) bool bIsADS;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) float SideInput;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) float ForwardInput;

	UPROPERTY(EditAnywhere, Replicated, Blueprintable) UDamageComponent* DamageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") AWeapon* PlayerWeapon;

	UFUNCTION() void PrintWasHit()
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was hit"), *this->GetName());
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
