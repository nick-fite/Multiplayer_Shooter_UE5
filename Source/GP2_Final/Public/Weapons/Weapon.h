// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Weapon.generated.h"



UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadDelegate);

class APlayerCharacter;

UCLASS()
class GP2_FINAL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetPlayerSkeletalMesh(APlayerCharacter* PlayerCharacter)
	{
		Player = PlayerCharacter; 
	}
		
	UFUNCTION() APlayerCharacter* Shoot();
	UFUNCTION(BlueprintCallable) void Reload();
	UFUNCTION(BlueprintCallable) int GetCurrentAmmo(){ return Ammo; }
	UFUNCTION(BlueprintCallable) int GetDefaultAmmo() { return DefaultAmmo; }
	UFUNCTION() int GetDamage() const { return Damage; }
	UFUNCTION() void PlayShootAnim();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere) UNiagaraComponent* Emitter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, Replicated,Category="Animations") UAnimMontage* PlayerReloadAnim;
private:
	UPROPERTY(EditAnywhere) APlayerCharacter* Player;
	UPROPERTY(EditAnywhere,Replicated, Category="Animations") UAnimMontage* PlayerShootAnim;
	UPROPERTY(EditAnywhere, Replicated,Category="Animations") UAnimMontage* WeaponShootAnim;
	UPROPERTY(EditAnywhere, Replicated,Category="Animations") UAnimMontage* WeaponReloadAnim;
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int Damage {10};
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int DefaultAmmo {20};
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int Ammo {20};
	UPROPERTY(BlueprintAssignable) FReloadDelegate ReloadDelegate;
};
