// Shoot Them Up Game , All Rights Reserved.


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon,Display,TEXT("Fire!"));

	MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
	if(!GetWorld()) return;

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart,TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart,TraceEnd);
	
	const FVector ActualTraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	const FVector HitDirectionFromMuzzle = (ActualTraceEnd - GetMuzzleWorldLocation()).GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(ActualTraceEnd,HitDirectionFromMuzzle));
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	UE_LOG(LogBaseWeapon, Display, TEXT("Shooting angle: %.0f"), Degrees);
	if(Degrees > 90.0f)
	{
		UE_LOG(LogBaseWeapon, Error, TEXT("Attempted to shoot backwards!"));
		return;
	}
		
	if(HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(),HitResult.ImpactPoint,FColor::Red,false,3.0f,0,3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		
		
		UE_LOG(LogBaseWeapon,Display,TEXT("Bone: %s"),*HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(),TraceEnd,FColor::Red,false,3.0f,0,3.0f);
	}
} \

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if(!Player) return nullptr;

	return Player->GetController<APlayerController>();
	
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if(!Controller) return false;
	
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart,FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation,ViewRotation) )return false;
	
	TraceStart  = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if(!GetWorld()) return;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if(!DamagedActor) return;

	DamagedActor->TakeDamage(HitDamage ,FDamageEvent{},GetPlayerController(), this);
}