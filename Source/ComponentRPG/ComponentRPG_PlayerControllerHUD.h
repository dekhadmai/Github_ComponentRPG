// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_CharacterHUD.h"
#include "ComponentRPG_SkillDeckHUD.h"
#include "ComponentRPG_PlayerControllerHUD.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_PlayerControllerHUD : public UComponentRPG_CharacterHUD
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SetHUDPos() override;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual void SetWidgetOwner(UObject* owner) override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetSkillDeckHUD(UComponentRPG_UserWidget* deck);

	UFUNCTION()
		void RefreshSkillDeckHUD_CheckEnable();
	
	UPROPERTY()
	UComponentRPG_SkillDeckHUD* SkillDeckHUD_UI;
};
