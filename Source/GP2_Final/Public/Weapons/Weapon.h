// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Weapon.generated.h"


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

	UFUNCTION(BlueprintCallable) void Shoot();
	UFUNCTION(BlueprintCallable) void Reload();
	
	UPROPERTY(EditAnywhere) UNiagaraComponent* Emitter;
private:	
	UPROPERTY(EditAnywhere) APlayerCharacter* Player;
	UPROPERTY(EditAnywhere) USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* PlayerShootAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* WeaponShootAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* WeaponReloadAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* PlayerReloadAnim;
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int Damage {10};
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int DefaultAmmo {20};
	UPROPERTY(EditAnywhere, Category="Weapon Stats") int Ammo {20};
};
