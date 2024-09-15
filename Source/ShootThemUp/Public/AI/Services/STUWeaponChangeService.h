// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUWeaponChangeService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUWeaponChangeService : public UBTService
{
	GENERATED_BODY()

public:
    USTUWeaponChangeService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.6f;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
