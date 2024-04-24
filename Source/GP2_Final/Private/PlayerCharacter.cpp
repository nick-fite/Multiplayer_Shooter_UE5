// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DamageComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	//these 2 lines, and the same ones in begin play are the reason why client side movement works. it took... 7 hours to
	//find this.
	//https://forums.unrealengine.com/t/attaching-player-to-actor-not-replicating-movement-properly/1430038/9
	//it took this person a month to find the same thing
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
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
		SpawnedCrosshair->AddToViewport(999);
		SpawnedCrosshair->AttachedPlayer = this;
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

	AddMovementInput(input.Y * GetMoveForwardDir() + input.X * GetMoveRightDir());

	SideInput = input.X;
	ForwardInput = input.Y;
	if(bIsSprinting)
	{
		SideInput *= 2;
		ForwardInput *= 2;
	}
	/*if()
	{
		GetController()->OnRep_ReplicatedMovement();
	}*/
}

void APlayerCharacter::MoveRPC_Implementation(const FVector DestLocation)
{
	GetCapsuleComponent()->SetWorldLocation(DestLocation);
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D input = InputValue.Get<FVector2D>();
	AddControllerYawInput(input.X * LookSensitivity);
	AddControllerPitchInput(-input.Y * LookSensitivity);

	/*if(!GetController()->GetPawn()->IsLocallyControlled())
	{
		CapsuleRot = GetCapsuleComponent()->GetComponentRotation();
	}
	else
	{
		LookRPC_Implementation(CapsuleRot);
	}*/
}

void APlayerCharacter::LookRPC_Implementation(const FRotator newRot)
{
	GetCapsuleComponent()->SetWorldRotation(newRot);
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


void APlayerCharacter::Shoot(const FInputActionValue& InputValue)
{
	PlayerWeapon->Shoot();
}

void APlayerCharacter::Reload(const FInputActionValue& inputValue)
{
	PlayerWeapon->Reload();
}

void APlayerCharacter::Sprint(const FInputActionValue& InputValue)
{
		bIsSprinting = InputValue.Get<bool>();
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

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, MappingContext);
	DOREPLIFETIME(APlayerCharacter, MoveAction);
	DOREPLIFETIME(APlayerCharacter, SprintAction);
	DOREPLIFETIME(APlayerCharacter, JumpAction);
	DOREPLIFETIME(APlayerCharacter, bIsSprinting);
	DOREPLIFETIME(APlayerCharacter, LookAction);
	DOREPLIFETIME(APlayerCharacter, ReloadAction);
	DOREPLIFETIME(APlayerCharacter, LookSensitivity);
	DOREPLIFETIME(APlayerCharacter, SideInput);
	DOREPLIFETIME(APlayerCharacter, ForwardInput);
	DOREPLIFETIME(APlayerCharacter, bIsADS);
	DOREPLIFETIME(APlayerCharacter, DamageComponent);
	DOREPLIFETIME(APlayerCharacter, CapsuleLoc);
	DOREPLIFETIME(APlayerCharacter,CapsuleRot)
}
