// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Components\BoxComponent.h"
#include "HealthComponent.h"
#include "Cannon.h"
#include "TankFactory.h"

// Sets default values
AMachinePawn::AMachinePawn()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AMachinePawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMachinePawn::DamageTaked);
}

void AMachinePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AMachinePawn::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void AMachinePawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}

	if (Cannon)
	{
		Cannon->Destroy();
	}

	EquippedCannonClass = newCannonClass;

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(EquippedCannonClass, params);
	Cannon->SetOwner(this);
	Cannon->ScoreChanged.AddUObject(this, &AMachinePawn::ShowScore);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);

}

float AMachinePawn::GetPoints()
{
	return ScoreValue;
}

void AMachinePawn::ShowScore(float Value)
{
	Score += Value;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Score: %f"), Score));
}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(EquippedCannonClass);
}

void AMachinePawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void AMachinePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s take Damage: %f,  Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}
