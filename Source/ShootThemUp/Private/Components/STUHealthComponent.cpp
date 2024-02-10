// Shoot Them Up Game. All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include <cmath>

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    OnDamageStop();
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    // GetOwner()->GetWorldTimerManager().SetTimer(;

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
    LastHitTime = FDateTime::Now();
    AutoHeal = false;

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
}

void USTUHealthComponent::OnDamageStop()
{
    //    float Milliseconds;
    //    std::modf(HealDelay, &Milliseconds);

    //    int32 Seconds = dynamic_cast<int32>(HealDelay);
    //    int32 MSeconds = dynamic_cast<int32>(Milliseconds);

    if ((FDateTime::Now() - LastHitTime) >= FTimespan::FromMilliseconds(HealDelay * 1000))
    {
        AutoHeal = true;
        UE_LOG(LogHealthComponent, Display, TEXT("Autoheal = %d"), AutoHeal);
    }
}

void USTUHealthComponent::OnHealthRestoration()
{
    if (Health == MaxHealth)
    {
        return;
    }
    if (AutoHeal)
    {
        UE_LOG(LogHealthComponent, Display, TEXT("You are regenerating!"));
    }
}
