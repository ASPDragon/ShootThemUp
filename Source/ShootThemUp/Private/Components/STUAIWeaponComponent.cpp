// Shoot Them Up Game. All Rights Reserved


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    if (CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;

    int32 NextIndex = (CurrentWeaponIndex + 1) % Armory.Num();

    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Armory[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (CurrentWeaponIndex + 1) % Armory.Num();
    }

    if (CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}