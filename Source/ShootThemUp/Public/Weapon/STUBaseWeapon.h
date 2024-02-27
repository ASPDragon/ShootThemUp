// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;
	
	virtual void StartFire();
    virtual void StopFire();
    
    void ChangeClip();
    bool CanReload() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float HeadshotMultiplier = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo { 30, 10, false };

	virtual void BeginPlay() override;

    virtual void MakeShot();
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& FViewRotation) const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    FVector GetMuzzleWorldLocation() const;

    void DecreaseAmmo();
    void LogAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;

private:
    FAmmoData CurrentAmmo;
};
