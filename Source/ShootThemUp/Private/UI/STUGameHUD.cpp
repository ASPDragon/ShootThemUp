// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
    const FVector2D Center = FVector2D(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.X - HalfLineSize, Center.Y, Center.X + HalfLineSize, Center.Y, LineColor, LineThickness);
    DrawLine(Center.X, Center.Y - HalfLineSize, Center.X, Center.Y + HalfLineSize, LineColor, LineThickness);
}
