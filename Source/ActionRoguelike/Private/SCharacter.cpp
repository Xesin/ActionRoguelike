// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, value);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(SecondaryProjectileClass);
}

void ASCharacter::DashAbility_TimeElapsed()
{
	SpawnProjectile(DashAbilityClass);
}

void ASCharacter::PrimaryAttack()
{
	if(ensure(AttackAnim))
		PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_MontageAnimation, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f); 
}

void ASCharacter::SecondaryAttack()
{
	if(ensure(SecondaryAttackAnim))
		PlayAnimMontage(SecondaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_MontageAnimation, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f); 
}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::DashAbility()
{
	if(ensure(AttackAnim))
		PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_MontageAnimation, this, &ASCharacter::DashAbility_TimeElapsed, 0.2f); 
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if(ensure(ClassToSpawn))
	{
		FVector StartPos = CameraComp->GetComponentLocation();
		FVector RayDirection = CameraComp->GetForwardVector();
	
		FVector EndPos = StartPos + (RayDirection * 4000);
	
		FHitResult Hit;
		FRotator ProjectileRotation = FQuat::Identity.Rotator();
		FVector ProjectileSpawnPos = GetMesh()->GetSocketLocation("Muzzle_01");
		bool BlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_Visibility);
	
		if(BlockingHit)
		{
			ProjectileRotation = FRotationMatrix::MakeFromX(Hit.Location - ProjectileSpawnPos).Rotator();
		}
		else
		{
			ProjectileRotation = FRotationMatrix::MakeFromX(EndPos - ProjectileSpawnPos).Rotator();
		}
	
		FTransform SpawnTM = FTransform(ProjectileRotation ,ProjectileSpawnPos);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("DashAbility", EInputEvent::IE_Pressed, this, &ASCharacter::DashAbility);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASCharacter::Jump);
}

