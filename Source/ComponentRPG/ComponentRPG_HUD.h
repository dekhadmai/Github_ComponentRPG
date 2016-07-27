// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ComponentRPG_HUD.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API AComponentRPG_HUD : public AHUD
{
	GENERATED_BODY()
	
		AComponentRPG_HUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;
	
	FString GetProjectileInfo(AComponentRPG_BaseProjectile* tmpProj);
	FString GetCharInfo(AComponentRPGCharacter* tmpChar);
};
