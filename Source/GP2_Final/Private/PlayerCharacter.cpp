// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "hud/Crosshair.h"
#include "Weapons/Weapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	ViewCamera = CreateDefaultSubobject<UCameraComponent>("View Camera");

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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn, SocketLoc, socketRotate);
		SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"hand_r_weapon_socket");
	}

	if(CrosshairToSpawn)
	{
		SpawnedCrosshair = CreateWidget<UCrosshair>(this, UCrosshair::StaticClass());	
		SpawnedCrosshair->AddToViewport(999);
	}
	
	CameraBoom->SetRelativeLocation(SpringArmPosition);
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
		
	}
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();
	AddMovementInput(input.Y * GetMoveForwardDir() + input.X * GetMoveRightDir());

}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D input = InputValue.Get<FVector2D>();
	AddControllerYawInput(input.X * LookSensitivity);
	AddControllerPitchInput(-input.Y * LookSensitivity);

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