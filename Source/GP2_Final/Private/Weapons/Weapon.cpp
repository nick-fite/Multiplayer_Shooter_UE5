// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "DamageComponent.h"
#include "PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	/*Emitter = CreateDefaultSubobject<UNiagaraComponent>("Emitter");
    USkeletalMeshSocket const* socket = SkeletalMesh->GetSocketByName("EmitterSocket");
	if(socket)
	{
		Emitter = CreateDefaultSubobject<UNiagaraComponent>("Emitter");
		Emitter->SetupAttachment(SkeletalMesh, socket->SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Socket"));
	}*/
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

void AWeapon::Shoot()
{
	if(!SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(nullptr) && Ammo > 0)
	{
		SkeletalMesh->GetAnimInstance()->Montage_Play(WeaponShootAnim);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerShootAnim);
		Ammo--;
		if(APlayerController* playerController = Cast<APlayerController>(Player->GetController()))
		{
			//Emitter->BeginPlay();
			UE_LOG(LogTemp, Warning, TEXT("working"));
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
				player->DamageComponent->ChangeHealth(Damage * -1);
				UE_LOG(LogTemp, Warning, TEXT("Hit player: %d"), player->DamageComponent->GetHealth());
				if(player->DamageComponent->GetHealth() <= 0)
				{
					player->DamageComponent->Kill();
				}
				player->PrintWasHit();
			}
		}
	}
}

void AWeapon::Reload()
{
	//if(SkeletalMesh->GetAnimInstance()->Montage_IsPlaying(nullptr))
	//{
		UE_LOG(LogTemp, Warning, TEXT("Reloading"));
		SkeletalMesh->GetAnimInstance()->Montage_Play(WeaponReloadAnim);
		Player->GetMesh()->GetAnimInstance()->Montage_Play(PlayerReloadAnim);
		Ammo = DefaultAmmo;
		ReloadDelegate.Broadcast();
	//}
}

