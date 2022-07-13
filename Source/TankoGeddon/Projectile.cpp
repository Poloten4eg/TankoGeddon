// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components\SceneComponent.h"
#include "Components\StaticMeshComponent.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "Scorable.h"


AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComponent;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(SceneComponent);
    ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
    ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
    GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);
    GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &AProjectile::Deactivate, DeactivateTime, false);
}

void AProjectile::Deactivate()
{
    bIsActivation = false;
    SetActorLocation(FVector(0.0f, 0.0f, -50.0f));
    GetWorld()->GetTimerManager().ClearTimer(DeactivateTimer);
    GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
    SetActorEnableCollision(false);
}

void AProjectile::Move()
{
    FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
    SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Projectile collided with %s, collided with component %s"), *OtherActor->GetName(), *OverlappedComp->GetName());

    AActor* owner = GetOwner();
    AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; 

    if (OtherActor != owner && OtherActor != ownerByOwner)
    {
        IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);
        IScorable* ScorableActor = Cast<IScorable>(OtherActor);
       
        float ScoreValue = 0.0f;

        if (ScorableActor)
        {
            ScoreValue = ScorableActor->GetPoints();
        }

        if (damageTakerActor)
        {
            FDamageData damageData;
            damageData.DamageValue = Damage;
            damageData.Instigator = owner;
            damageData.DamageMaker = this;

            damageTakerActor->TakeDamage(damageData);

            if (OtherActor->IsActorBeingDestroyed() && ScoreValue != 0.0f)
            {
                if (OnKilled.IsBound())
                {
                    OnKilled.Broadcast(ScoreValue);
                }
            }
        }
        else
        {
            OtherActor->Destroy();
        }

        Deactivate();
    }

}

