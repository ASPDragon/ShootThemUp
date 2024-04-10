// Shoot Them Up Game. All Rights Reserved


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float ClosestDistance = MAX_FLT;
    AActor* ClosestPawn = nullptr;
    for (const auto PerceivedActor : PerceivedActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerceivedActor);
        if (HealthComponent && !HealthComponent->IsDead()) // TODO Check if enemies or not
        {
            const auto CurrentDistance = (PerceivedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < ClosestDistance)
            {
                ClosestDistance = CurrentDistance;
                ClosestPawn = PerceivedActor;
            }
        }
    }

    return ClosestPawn;
}