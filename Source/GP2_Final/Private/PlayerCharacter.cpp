// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DamageComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GS_MultiplayerState.h"
#include "MultiplayerGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "hud/Crosshair.h"
#include "Weapons/Weapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	CameraBoom =  CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	ViewCamera = CreateDefaultSubobject<UCameraComponent>("View Camera");

	bReplicates = true;
	
	CameraBoom->SetupAttachment(GetRootComponent());
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//FVector currentCamLoc = CameraBoom->GetComponentLocation();
	//FVector output;
	//FMath::Lerp(currentCamLoc, SpringArmPosition, output);
	CameraBoom->SetRelativeLocation(SpringArmPosition);
	
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 200.f;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1080.f);
	GetCharacterMovement()->JumpZVelocity = 400.f;

	DamageComponent = CreateDefaultSubobject<UDamageComponent>("Damage Component");

	GetMesh()->SetIsReplicated(true);
	//these single line, and the same one in begin play is the reason why client side movement works. it took... 7 hours to
	//find this.
	//https://forums.unrealengine.com/t/attaching-player-to-actor-not-replicating-movement-properly/1430038/9
	//it took this person a month to find the same thing
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DamageComponent->SetIsReplicated(true);
	bAlwaysRelevant = true;
	

	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = true;

	if(APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(MappingContext,0);
		}
	}

	if(WeaponToSpawn)
	{
		const FRotator socketRotate = GetMesh()->GetSocketRotation("hand_r_weapon_socket");
		const FVector SocketLoc = GetMesh()->GetSocketLocation("hand_r_weapon_socket");

		PlayerWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn, SocketLoc, socketRotate);
		PlayerWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"hand_r_weapon_socket");
		PlayerWeapon->SetPlayerSkeletalMesh(this);
	}

	if(CrosshairToSpawn)
	{
		SpawnedCrosshair = CreateWidget<UCrosshair>(GetWorld(), CrosshairToSpawn);	
		SpawnedCrosshair->AttachedPlayer = this;
		SetHudDelegate.Broadcast();
		SpawnedCrosshair->SetHud();
		if(IsLocallyControlled())
		{
			SpawnedCrosshair->AddToViewport(999);
		}
	}

	CameraBoom->SetRelativeLocation(SpringArmPosition);

	bUseControllerRotationYaw = true;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		enhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		enhancedInputComp->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		
		enhancedInputComp->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Shoot);
		enhancedInputComp->BindAction(ADSAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ADS);
		enhancedInputComp->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Reload);
	}
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();

	SideInput = input.X;
	ForwardInput = input.Y;
	if(bIsSprinting)
	{
		SideInput *= 2;
		ForwardInput *= 2;
	}

	AddMovementInput(ForwardInput * GetMoveForwardDir() + SideInput * GetMoveRightDir());

	MoveRPC(ForwardInput, SideInput);
}

void APlayerCharacter::MoveRPC_Implementation(const float Forward, const float Right)
{
	SideInput = Right;
	ForwardInput = Forward;
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D input = InputValue.Get<FVector2D>();
	AddControllerYawInput(input.X * LookSensitivity);
	AddControllerPitchInput(-input.Y * LookSensitivity);

	LookPitch = GetBaseAimRotation().Pitch;
	LookRPC(LookPitch);
}

void APlayerCharacter::LookRPC_Implementation(const float Pitch)
{
	LookPitch = Pitch;
}

FVector APlayerCharacter::GetMoveForwardDir() const
{
	FVector cameraForward = ViewCamera->GetForwardVector();
	cameraForward.Z = 0;
	return cameraForward.GetSafeNormal();

}

FVector APlayerCharacter::GetMoveRightDir() const
{
	return ViewCamera->GetRightVector();
}

void APlayerCharacter::ADS(const FInputActionValue& InputValue)
{
	bIsADS = InputValue.Get<bool>();
	
	if (bIsADS)
	{
		bIsSprinting = true;
		UE_LOG(LogTemp, Warning, TEXT("ADS: true"));
	}
	else
	{
		bIsSprinting = false;
		UE_LOG(LogTemp, Warning, TEXT("ADS: False"));
	}
}

void APlayerCharacter::Shoot_Implementation(const FInputActionValue& InputValue)
{
	//code was originally implemented ENTIRELY within AWeapon but was failing to replicate.
	//Now weapon is used to get player that was hit, and then we replicate here.
	APlayerCharacter* hitplayer = PlayerWeapon->Shoot();
	if(IsValid(hitplayer))
	{
		ShootRPC(hitplayer, PlayerWeapon->GetDamage() * -1);
		CheckIfPlayerDeadServer(hitplayer);
	}

	SpawnedCrosshair->Shoot();
}

void APlayerCharacter::ShootRPC_Implementation(APlayerCharacter* damagedPlayer, int damage)
{
	ShootMulticast(damagedPlayer, damage);
}

void APlayerCharacter::ShootMulticast_Implementation(APlayerCharacter* damagedPlayer, int damage)
{
	damagedPlayer->DamageComponent->ChangeHealth(damage);
	UE_LOG(LogTemp, Warning, TEXT("%s was hit and is at %d health"), *damagedPlayer->GetName(), damagedPlayer->DamageComponent->GetHealth());
}

void APlayerCharacter::Reload(const FInputActionValue& inputValue)
{
	ReloadRPC();
}

void APlayerCharacter::ReloadRPC_Implementation()
{
	ClientReload();
}

void APlayerCharacter::ClientReload_Implementation()
{
	PlayerWeapon->Reload();
	SpawnedCrosshair->Reload();
}

void APlayerCharacter::Sprint(const FInputActionValue& InputValue)
{
	bIsSprinting = InputValue.Get<bool>();
	

	SprintRPC_Implementation(bIsSprinting);
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	GetMesh()->GetAnimInstance()->Montage_Play(JumpMontage);
}

void APlayerCharacter::ClientSprint_Implementation(const bool newVal)
{
	bIsSprinting = newVal;
	if(bIsSprinting)
	{
		SideInput *= 2;
		ForwardInput *= 2;
	}
	else
	{
		SideInput /= 2;
		ForwardInput /= 2;
	}
}

void APlayerCharacter::SprintRPC_Implementation(const bool newVal)
{
	ClientSprint(newVal);
	if(bIsSprinting)
	{
		SideInput *= 2;
		ForwardInput *= 2;
	}
	else
	{
		SideInput /= 2;
		ForwardInput /= 2;
	}
	MoveRPC(ForwardInput, SideInput);
}

void APlayerCharacter::PrintWasHit_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s was hit"), *this->GetName());
}

void APlayerCharacter::KillPlayer()
{
	GetMesh()->SetSimulatePhysics(true);
	DisableInput(GetLocalViewingPlayerController());
}

void APlayerCharacter::CheckIfPlayerDeadMulticast_Implementation(APlayerCharacter* damagedPlayer)
{
	if(damagedPlayer->DamageComponent->GetHealth() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is dead"), *damagedPlayer->GetName());
		damagedPlayer->KillPlayer();
	}
}

void APlayerCharacter::CheckIfPlayerDeadServer_Implementation(APlayerCharacter* damagedPlayer)
{
	if(damagedPlayer->DamageComponent->GetHealth() <= 0)
 	{
 		UE_LOG(LogTemp, Warning, TEXT("%s is dead"), *damagedPlayer->GetName());
 		damagedPlayer->KillPlayer();
 	}
	CheckIfPlayerDeadMulticast(damagedPlayer);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, bIsSprinting);
	DOREPLIFETIME(APlayerCharacter, SideInput);
	DOREPLIFETIME(APlayerCharacter, ForwardInput);
	DOREPLIFETIME(APlayerCharacter, bIsADS);
	DOREPLIFETIME(APlayerCharacter, DamageComponent);
	DOREPLIFETIME(APlayerCharacter, LookPitch);
}

void APlayerCharacter::TestMulticast_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Multicast from cpp"));
}
