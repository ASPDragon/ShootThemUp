// Shoot Them Up Game. All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include "Camera/CameraShakeBase.h"
// #include <cmath>

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

	if (!AutoHeal)
    {
        StartAutoHeal();
    }
    else
    {
        if (FDateTime::Now() - LastHealTime >= FTimespan::FromSeconds(HealUpdateTime))
        {
            OnHealthRestoration();
            LastHealTime = FDateTime::Now();
        }
    }
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsHealthFull() || IsDead() || HealthAmount <= 0) return false;
    
    SetHealth(Health + HealthAmount);
    UE_LOG(LogHealthComponent, Display, TEXT("Health is added!"));

    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);
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

    SetHealth(Health - Damage);
    LastHitTime = FDateTime::Now();
    AutoHeal = false;

    if (IsDead())
    {
        OnDeath.Broadcast();
    }

    PlayCameraShake();
}

void USTUHealthComponent::StartAutoHeal()
{
    if ((FDateTime::Now() - LastHitTime) <= FTimespan::FromSeconds(HealDelay) || IsDead())
    {
        return;
    }
    AutoHeal = true;
}

void USTUHealthComponent::OnHealthRestoration()
{
    if (IsHealthFull())
    {
        AutoHeal = false;
    }
    if (AutoHeal)
    {
		// Health + (FPS * HealthModifier) * DeltaTime
        SetHealth(Health + HealModifier);
        UE_LOG(LogHealthComponent, Display, TEXT("Healed %f"), HealModifier);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth) {
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
