// Shoot Them Up Game , All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

void ASTURifleWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
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