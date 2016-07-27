// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillButtonWidget.h"
#include "ComponentRPG_TargetingWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_TargetingWidget : public UComponentRPG_SkillButtonWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetTargetingType(TargetingType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetTargetSideType(TargetSideType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetTargetType(TargetType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ECheckBoxState GetTargetingType(TargetingType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ECheckBoxState GetTargetSideType(TargetSideType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ECheckBoxState GetTargetType(TargetType val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ECheckBoxState GetIsSelectingTarget();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SelectTargetButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetIsSelectingTarget(bool val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		bool GetIsCharacterButtonEnable();
};
