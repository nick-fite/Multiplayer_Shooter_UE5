// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "DamageComponent.h"
#include "PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	bAlwaysRelevant = true;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APlayerCharacter* AWeapon::Shoot()
{
	if(!SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(nullptr) && Ammo > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling Shoot"));
		
		SkeletalMesh->GetAnimInstance()->Montage_Play(WeaponShootAnim);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerShootAnim);
		Ammo--;
		if(APlayerController* playerController = Cast<APlayerController>(Player->GetController()))
		{
			int viewportX, viewportY;
			playerController->GetViewportSize(viewportX, viewportY);
			
			FVector worldLoc, worldDir;
			playerController->DeprojectScreenPositionToWorld(viewportX * 0.5f, viewportY * 0.5f, worldLoc, worldDir);

			worldLoc += worldDir * 100.0;
			FVector endPos = worldLoc + worldDir * 999.f;

			FHitResult hit;
			GetWorld()->LineTraceSingleByChannel(hit, worldLoc, endPos, ECC_WorldDynamic);
			DrawDebugLine(GetWorld(), SkeletalMesh->GetComponentLocation(), endPos, FColor::Green, true);
			
			if(APlayerCharacter* player = Cast<APlayerCharacter>(hit.GetActor()))
			{
				return  player;
			}
			return  nullptr;
		}
	}
	return nullptr;
}

void AWeapon::Reload()
{
	if(!SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading"));
		SkeletalMesh->GetAnimInstance()->Montage_Play(WeaponReloadAnim);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerReloadAnim);
		Ammo = DefaultAmmo;
		ReloadDelegate.Broadcast();
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, PlayerReloadAnim);
	DOREPLIFETIME(AWeapon, PlayerShootAnim);
	DOREPLIFETIME(AWeapon, WeaponReloadAnim);
	DOREPLIFETIME(AWeapon, WeaponShootAnim);
}
