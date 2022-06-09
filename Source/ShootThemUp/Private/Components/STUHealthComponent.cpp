// Shoot Them Up Game , All Rights Reserved.


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);	
	}

	
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <=0.0f || IsDead()	|| !GetWorld() ) return;
	
	SetHealth(Health - Damage );
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	
	if (IsDead())
	{
		OnDeath.Broadcast();
	}

	else if (AutoHeal == true )
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate,HealUpdateTime,true,HealDelay);
	}
	
}

void USTUHealthComponent::HealUpdate()
{
		SetHealth(Health + HealModifier);

		if(FMath::IsNearlyEqual( Health, MaxHealth )&& GetWorld() )
		{
			GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth,0.0f,MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
