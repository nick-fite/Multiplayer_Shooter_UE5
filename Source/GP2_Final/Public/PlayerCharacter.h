// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.generated.h"


class UWidget;
class UWidgetComponent;
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetHudDelegate);

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
	UFUNCTION(Server, Reliable) void MoveRPC(const float Forward, const float Right);
	UFUNCTION() void Look(const FInputActionValue& InputValue);
	UFUNCTION(Server, Reliable) void LookRPC(const float Pitch);
	
	UFUNCTION() FVector GetMoveForwardDir() const;
	UFUNCTION() FVector GetMoveRightDir() const;

	UFUNCTION() void ADS(const FInputActionValue& InputValue);
	UFUNCTION(Client, Reliable) void Shoot(const FInputActionValue& InputValue);
	UFUNCTION(Server, Reliable) void ShootRPC(APlayerCharacter* damagedPlayer, int damage);
	UFUNCTION(NetMulticast, Reliable) void ShootMulticast(APlayerCharacter* damagedPlayer, int damage);
	UFUNCTION(Server, Reliable) void PlayShootAnimServer();
	UFUNCTION(NetMulticast, Reliable) void PlayShootAnimMulticast(APlayerCharacter* playerToAnim);
	UFUNCTION() void Reload(const FInputActionValue& inputValue);
	UFUNCTION(Server, Reliable) void ReloadRPC();
	UFUNCTION(NetMulticast, Reliable) void ClientReload();
	UFUNCTION() void Sprint(const FInputActionValue& InputValue);
	UFUNCTION(Server, Reliable) void SprintRPC(const bool newVal);
	UFUNCTION(NetMulticast, Reliable) void ClientSprint(const bool newVal);
	virtual void Jump() override;
	UFUNCTION(Server, Reliable) void JumpServerRPC();

	//camera
	UPROPERTY(VisibleAnywhere, Category="View") USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category="View") UCameraComponent* ViewCamera;
	UPROPERTY(EditAnywhere, Category="View") FVector SpringArmPosition;	

	UPROPERTY(EditAnywhere, Category = "Input") UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, category="Input") UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, category="Input") UInputAction* JumpAction;
	UPROPERTY(EditAnywhere,Replicated, Category="Input") bool bIsSprinting;
	
	UPROPERTY(EditAnywhere, category="Input") UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* ADSAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, Category="Input") UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Category = "Input") float LookSensitivity {0.5f};

	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<AWeapon> WeaponToSpawn;

	UPROPERTY(EditAnywhere, Category= "Animations") UAnimMontage* JumpMontage;
	
	UPROPERTY(EditAnywhere, Category="Weapon") TSubclassOf<UCrosshair> CrosshairToSpawn;
	UPROPERTY(EditAnywhere, Category="Weapon") UCrosshair* SpawnedCrosshair;

	UPROPERTY(EditAnywhere, Category="ADS") float DefaultFOV{90.f};
	UPROPERTY(EditAnywhere, Category="ADS") float ADS_FOV{45.f};

public:
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) bool bIsADS;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) float SideInput;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) float ForwardInput;

	UPROPERTY(EditAnywhere,Replicated,BlueprintReadWrite) float LookPitch;
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) UDamageComponent* DamageComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") AWeapon* PlayerWeapon;

	UPROPERTY() FSetHudDelegate SetHudDelegate;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite,Category="Widgets") UWidgetComponent* HealthWidget;

	UPROPERTY(EditAnywhere, ReplicatedUsing=PlayName_OnRep) FString PlayerName;

	UFUNCTION() void PlayName_OnRep(const FString& NewName);
	
	UFUNCTION(NetMulticast, Reliable) void PrintWasHit();

	UFUNCTION(BlueprintCallable) void KillPlayer();
	UFUNCTION(NetMulticast, Reliable) void CheckIfPlayerDeadMulticast(APlayerCharacter* damagedPlayer);
	UFUNCTION(Server, Reliable) void CheckIfPlayerDeadServer(APlayerCharacter* damagedPlayer);

	UFUNCTION(Client, Reliable) void SetNameClient(const FString& Name);
	UFUNCTION(Server, Reliable) void SetNameServer(const FString& Name);
	//UFUNCTION(NetMulticast, Reliable) void SetNameNetMulticast(const FString& Name);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
