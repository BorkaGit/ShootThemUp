// Shoot Them Up Game , All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{ 
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();	

	check(WeaponFXComponent);
}


void ASTURifleWeapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);\
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
	if(!GetWorld() || IsAmmoEmpty()) return;

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart,TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart,TraceEnd);
	
	/*const FVector ActualTraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	const FVector HitDirectionFromMuzzle = (ActualTraceEnd - GetMuzzleWorldLocation()).GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(ActualTraceEnd,HitDirectionFromMuzzle));
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	UE_LOG(LogBaseWeapon, Display, TEXT("Shooting angle: %.0f"), Degrees);
	if(Degrees > 90.0f)
	{
		UE_LOG(LogBaseWeapon, Error, TEXT("Attempted to shoot backwards!"));
		return;
	}*/

	FVector TraceFXEnd = TraceEnd;
	if(HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
		
		UE_LOG(LogBaseWeapon,Display,TEXT("Bone: %s"),*HitResult.BoneName.ToString());
	}
	SpawnTraceFX(GetMuzzleWorldLocation(),TraceFXEnd);
	
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart,FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation,ViewRotation) )return false;
	
	TraceStart  = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone( ViewRotation.Vector(), HalfRad );
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if(!DamagedActor) return;

	DamagedActor->TakeDamage(HitDamage ,FDamageEvent{},GetPlayerController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if(!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if(MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible,true);
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),TraceFX,TraceStart);
	if(TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}
