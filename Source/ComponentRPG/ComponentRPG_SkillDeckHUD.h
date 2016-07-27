// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_SkillComponentDeck.h"
#include "PanelWidget.h"
#include "ComponentRPG_SkillButtonWidget.h"
#include "ComponentRPG_TargetingWidget.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_SkillDeckHUD.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_SkillDeckHUD : public UComponentRPG_UserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		UComponentRPG_SkillComponentDeck* SkillDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillComponent)
		UPanelWidget* SkillTargetingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillComponent)
		UPanelWidget* SkillListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> ButtonWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> TargetingButtonWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> MoveCommandButtonWidget;

	void Init();
	
	void SetSkillDeck(UComponentRPG_SkillComponentDeck* deck);
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	virtual void RefreshWidgetData() override;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void SetVBox(UPanelWidget* skilltarget_vbox, UPanelWidget* skilllist_vbox);
	
};
