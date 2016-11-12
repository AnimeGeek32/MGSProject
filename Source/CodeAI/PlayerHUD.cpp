// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeAI.h"
#include "PlayerHUD.h"
#include "MGSCube.h"
#include "Kismet/KismetMathLibrary.h"

void APlayerHUD::BeginPlay()
{
	
}

void APlayerHUD::AddToRadar(AActor * NewActor)
{
	EnemiesArray.Add(NewActor);
}

void APlayerHUD::RemoveFromRadar(AActor * Actor)
{
	EnemiesArray.Remove(Actor);
}

FVector2D APlayerHUD::GetRadarCenterPosition()
{
	return FVector2D(ScreenSize.X * RadarStartLocation.X, ScreenSize.Y * RadarStartLocation.Y);
}

FVector2D APlayerHUD::GetRadarDotPosition(FVector Location)
{
	FTransform Trans = FTransform(FRotator(0.f, 0.f, 0.f),
								GetOwningPlayerController()->GetPawn()->GetActorLocation(),
								GetOwningPlayerController()->GetPawn()->GetActorScale3D());
	FVector ITL = UKismetMathLibrary::InverseTransformLocation(Trans, Location);
	ITL /= ObjectDistance;

	return FVector2D(ITL.X, ITL.Y);
}

void APlayerHUD::DrawBorder()
{
	DrawLine(GetRadarCenterPosition().X - RadarSize,
			GetRadarCenterPosition().Y - RadarSize,
			GetRadarCenterPosition().X - RadarSize,
			GetRadarCenterPosition().Y + RadarSize,
			FColor::Black, 2.f);
	DrawLine(GetRadarCenterPosition().X - RadarSize,
			GetRadarCenterPosition().Y + RadarSize,
			GetRadarCenterPosition().X + RadarSize,
			GetRadarCenterPosition().Y + RadarSize,
			FColor::Black, 2.f);
	DrawLine(GetRadarCenterPosition().X + RadarSize,
			GetRadarCenterPosition().Y + RadarSize,
			GetRadarCenterPosition().X + RadarSize,
			GetRadarCenterPosition().Y - RadarSize,
			FColor::Black, 2.f);
	DrawLine(GetRadarCenterPosition().X + RadarSize,
			GetRadarCenterPosition().Y - RadarSize,
			GetRadarCenterPosition().X - RadarSize,
			GetRadarCenterPosition().Y - RadarSize,
			FColor::Black, 2.f);
}

void APlayerHUD::DrawEnemies()
{
	for (AActor* Actor : EnemiesArray) {
		if (Actor->IsValidLowLevel() && !Actor->IsPendingKillPending()) {
			float x, y;
			//Math to draw the actor's location
			x = -GetRadarDotPosition(Actor->GetActorLocation()).X;
			y = GetRadarDotPosition(Actor->GetActorLocation()).Y;

			//If the enemy is outside the radar, don't draw it
			if (!(x < -RadarSize || x > RadarSize || y < -RadarSize || y > RadarSize)) {
				DrawRect(FColor::Black,
					y + GetRadarCenterPosition().X,
					x + GetRadarCenterPosition().Y,
					5.f, 5.f);
				if (EnemySight) {
					FLinearColor Color = FLinearColor::Blue;
					Color.A = .3f;
					DrawTexture(EnemySight,
						y + GetRadarCenterPosition().X - (EnemySight->GetSizeX() / 2 - 2.5f),
						x + GetRadarCenterPosition().Y - (EnemySight->GetSizeY() / 2 - 2.5f),
						EnemySight->GetSizeX(),
						EnemySight->GetSizeY(),
						EnemySight->GetSizeX(),
						EnemySight->GetSizeY(),
						1.f,
						1.f,
						Color,
						EBlendMode::BLEND_Translucent,
						1.f,
						false,
						Actor->GetActorRotation().Yaw,
						FVector2D(0.5f, 0.5f));
				}
			}
		}
	}
}

void APlayerHUD::DrawCubes()
{
	
}

void APlayerHUD::ContinueDrawHUD(int32 SizeX, int32 SizeY)
{
	ScreenSize.X = SizeX;
	ScreenSize.Y = SizeY;

	//DrawCubes();
	DrawEnemies();
	DrawRect(FColor::Blue, GetRadarCenterPosition().X, GetRadarCenterPosition().Y, 5.f, 5.f);
	DrawBorder();
	
}



