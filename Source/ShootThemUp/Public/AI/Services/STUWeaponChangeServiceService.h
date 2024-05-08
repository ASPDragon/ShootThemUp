// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUWeaponChangeServiceService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUWeaponChangeServiceService : public UBTService
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Probability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Delay;
};
