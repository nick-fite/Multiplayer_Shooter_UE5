// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "PlayerCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::PlayShootAnim()
{
	if(!SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		SkeletalMesh->GetAnimInstance()->Montage_Play(WeaponShootAnim);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerShootAnim);

		if(APlayerController* playerController = Cast<APlayerController>(Player->GetController()))
		{
			UE_LOG(LogTemp, Warning, TEXT("working"));
			int viewportX, viewportY;
			playerController->GetViewportSize(viewportX, viewportY);

			FVector worldLoc, worldDir;
			playerController->DeprojectScreenPositionToWorld(viewportX * 0.5f, viewportY * 0.5f, worldLoc, worldDir);

			worldLoc += worldDir * 100.0;
			FVector endPos = worldLoc + worldDir * 999.f;

			DrawDebugLine(GetWorld(), SkeletalMesh->GetComponentLocation(), endPos, FColor::Green, true);
			//FRotator newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), endPos);

			//this->SetActorRotation(FRotator(GetActorRotation().Pitch, newRot.Yaw + 20, GetActorRotation().Roll));
		}
	}
}

