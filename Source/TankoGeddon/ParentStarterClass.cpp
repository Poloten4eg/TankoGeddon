// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentStarterClass.h"

#include "HealthComponent.h"
#include "MachinePawn.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AParentStarterClass::AParentStarterClass()
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
}	

void AParentStarterClass::ReceivePoints(int InPoints)
{
	Points += InPoints;
	OnPointsReceived(InPoints);
}


void AParentStarterClass::BeginPlay()
{
	Super::BeginPlay();
}

void AParentStarterClass::Die()
{
}

void AParentStarterClass::DamageTaked(float DamageValue)
{
}

void AParentStarterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParentStarterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

