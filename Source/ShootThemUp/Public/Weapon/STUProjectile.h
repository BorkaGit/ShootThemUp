// Shoot Them Up Game , All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction)  { ShotDirection = Direction; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Weapon")
	UProjectileMovementComponent* MovementComponent;
	
	virtual void BeginPlay() override;

private:
	FVector ShotDirection;
};
