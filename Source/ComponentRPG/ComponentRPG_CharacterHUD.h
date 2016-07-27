// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Blueprint/UserWidget.h"
//#include "ProgressBar.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_CharacterHUD : public UComponentRPG_UserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void RefreshWidgetData() override;

	virtual void SetHUDPos() override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ESlateVisibility IsSelectedByController();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		ESlateVisibility IsActionBarFull();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		float GetPercentActionBar();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetActionBarString();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		float GetPercentCastBar();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetCastBarString();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		float GetPercentHP();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetHPString();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		float GetPercentMP();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetMPString();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Damage();
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Effectiveness();
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Defense();
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Resistance();
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Speed();
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetStatPointString_Recovery();
};
