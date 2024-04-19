// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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
	void SetPlayerSkeletalMesh(USkeletalMeshComponent* mesh)
	{
		playerMesh = mesh; 
	}

	UFUNCTION(BlueprintCallable) void PlayShootAnim();

	
private:	
	UPROPERTY(EditAnywhere) USkeletalMeshComponent* playerMesh;
	UPROPERTY(EditAnywhere) USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* PlayerShootAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* WeaponShootAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* WeaponReloadAnim;
	UPROPERTY(EditAnywhere, Category="Animations") UAnimMontage* PlayerReloadAnim;
};
