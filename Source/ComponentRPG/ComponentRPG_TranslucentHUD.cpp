// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_TranslucentHUD.h"


void UComponentRPG_TranslucentHUD::NativeConstruct()
{
	Super::NativeConstruct();

	TileSize = 32.0f * 4.0f;
}

void UComponentRPG_TranslucentHUD::SetHUDPos()
{
	if (!myController || !myOwner || !IndicatorImage)
	{
		return;
	}

	if (myController && myOwner)
	{
		if (myOwner)
		{
			// stick to character
			FVector2D result;
			Cast<AComponentRPG_PlayerController>(myController)->ProjectWorldLocationToScreen(Cast<AComponentRPGCharacter>(myOwner)->GetActorLocation(), result);
			SetPositionInViewport(result + (IndicatorImage->Brush.ImageSize.X / -3.0f));
		}
	}
}

void UComponentRPG_TranslucentHUD::SetHUDPos(FVector worldPos)
{
	FVector2D result;
	Cast<AComponentRPG_PlayerController>(myController)->ProjectWorldLocationToScreen(worldPos, result);
	SetPositionInViewport(result + (IndicatorImage->Brush.ImageSize.X / -3.0f));
}


void UComponentRPG_TranslucentHUD::SetWidgetOwner(UObject* owner)
{
	Super::SetWidgetOwner(owner);

	if (owner != nullptr)
		SetHUDPos();
}

void UComponentRPG_TranslucentHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetHUDPos();
}

void UComponentRPG_TranslucentHUD::SetIndicatorImage(UImage* img)
{
	IndicatorImage = img;
}

void UComponentRPG_TranslucentHUD::SetSquareIndicator()
{
	IndicatorImage->Brush.SetResourceObject(nullptr);
}
void UComponentRPG_TranslucentHUD::SetCircleIndicator()
{
	IndicatorImage->Brush.SetResourceObject(CircleTexture);
}

void UComponentRPG_TranslucentHUD::SetIndicatorSize(float width, float height)
{
	IndicatorImage->Brush.ImageSize = FVector2D(width * TileSize, height * TileSize);
}
void UComponentRPG_TranslucentHUD::SetIndicatorColor(FLinearColor val)
{
	IndicatorColor = val;
}

FLinearColor UComponentRPG_TranslucentHUD::GetIndicatorColor()
{
	return IndicatorColor;
}