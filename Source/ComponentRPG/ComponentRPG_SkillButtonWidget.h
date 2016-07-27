// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_SkillComponent.h"
#include "ComponentRPG_SkillButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_SkillButtonWidget : public UComponentRPG_UserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UComponentRPG_SkillComponent* SkillComponent;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetSkillComponent(UComponentRPG_SkillComponent* comp);
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetText();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual UComponentRPG_UserWidget* GetTooltipWidget() override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void GetIcon();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ECheckBoxState GetIsSelected();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		bool GetIsEnabled();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void OnButtonClicked();
};
