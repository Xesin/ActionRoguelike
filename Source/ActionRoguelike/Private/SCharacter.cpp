// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SAttributesComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

	AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = CameraComp->GetComponentLocation();
	OutRotation = GetControlRotation();
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
	GetWorldTimerManager().ClearTimer(TimerHandle_MontageAnimation);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(SecondaryProjectileClass);
	GetWorldTimerManager().ClearTimer(TimerHandle_MontageAnimation);
}

void ASCharacter::DashAbility_TimeElapsed()
{
	SpawnProjectile(DashAbilityClass);
	GetWorldTimerManager().ClearTimer(TimerHandle_MontageAnimation);
}

void ASCharacter::PrimaryAttack()
{
	if (TimerHandle_MontageAnimation.IsValid()) return;

	if (ensure(PrimaryCastVFX))
	{
		UGameplayStatics::SpawnEmitterAttached(PrimaryCastVFX, GetMesh(), PrimaryCastSocket);
	}

	if(ensure(AttackAnim))
		PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_MontageAnimation, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f); 
}

void ASCharacter::SecondaryAttack()
{
	if (TimerHandle_MontageAnimation.IsValid()) return;

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

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			DisableInput(PC);
		}
	}
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if(ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

	
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector RayDirection = GetControlRotation().Vector();
		FVector TraceEnd = TraceStart + (RayDirection * 5000);
	
		FHitResult Hit;
	
		if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
	
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation ,HandLocation);		
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
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

