// Shoot Them Up Game , All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtiles.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup,All,All);

bool ASTUAmmoPickup::GivenPickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = STUUtiles::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if(!HealthComponent || HealthComponent->IsDead() ) return false;

	
	const auto WeaponComponent = STUUtiles::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
	if(!WeaponComponent ) return false;
	
	return WeaponComponent->TryToAddAmmo(WeaponType,ClipsAmount);
}
