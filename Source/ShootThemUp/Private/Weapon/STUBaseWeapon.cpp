// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire() {}

void ASTUBaseWeapon::StopFire() {}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("No more clips."));
            return;
        }
        
        CurrentAmmo.Clips--;
    }
    
    CurrentAmmo.BulletsInCurrentClip = CurrentAmmo.BulletsPerClip;
    UE_LOG(LogBaseWeapon, Display, TEXT("------ Change Clip ------"));
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.BulletsInCurrentClip < DefaultAmmo.BulletsInCurrentClip && CurrentAmmo.Clips > 0;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        if (const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount; DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added!"));
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.BulletsInCurrentClip = CurrentAmmo.BulletsPerClip;
            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now!"));
        }
    }
    else
    {
        CurrentAmmo.BulletsInCurrentClip = CurrentAmmo.BulletsPerClip;
        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added!"));
    }
    
    return true;
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.BulletsInCurrentClip > 0, TEXT("Bullets count couldn't be less or equal to zero."));
    checkf(DefaultAmmo.BulletsPerClip > 0, TEXT("Bullets count in clip couldn't be less or equal to zero."));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal to zero."));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot() {}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    const FVector MuzzlePosition = GetMuzzleWorldLocation();
/*    const float AngleInDegrees = FMath::Acos(FVector::DotProduct(MuzzlePosition, TraceStart));

    if (AngleInDegrees > 90.0f) return; */

    // if (TraceEnd.Z <= MuzzlePosition.Z) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());

    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();

    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.BulletsInCurrentClip == 0)
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Clip is empty."));
        return;
    }
    
    CurrentAmmo.BulletsInCurrentClip--;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsInCurrentClip) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.BulletsInCurrentClip == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.BulletsInCurrentClip == CurrentAmmo.BulletsPerClip;
}
