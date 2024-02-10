// Shoot Them Up Game. All Rights Reserved

#include "STUGameModeBase.h"
#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
}