// Shoot Them Up Game , All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtiles.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup,All,All);

bool ASTUHealthPickup::GivenPickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = STUUtiles::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if(!HealthComponent) return false;
	
	return HealthComponent->TryToAddHealth(HealthAmount);
}