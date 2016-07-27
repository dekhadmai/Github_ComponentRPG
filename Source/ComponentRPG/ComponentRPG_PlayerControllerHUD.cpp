// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerControllerHUD.h"


void UComponentRPG_PlayerControllerHUD::SetHUDPos()
{
	
}

void UComponentRPG_PlayerControllerHUD::SetWidgetOwner(UObject* owner)
{
	Super::SetWidgetOwner(owner);

	if (SkillDeckHUD_UI != nullptr)
	{
		RefreshSkillDeckHUD_CheckEnable();

		SkillDeckHUD_UI->SetSkillDeck(Cast<AComponentRPGCharacter>(owner)->SkillComponentDeck);
		SkillDeckHUD_UI->SetController(myController);
	}
}

void UComponentRPG_PlayerControllerHUD::SetSkillDeckHUD(UComponentRPG_UserWidget* deck)
{
	SkillDeckHUD_UI = Cast<UComponentRPG_SkillDeckHUD>(deck);
}

void UComponentRPG_PlayerControllerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshSkillDeckHUD_CheckEnable();
}

void UComponentRPG_PlayerControllerHUD::RefreshSkillDeckHUD_CheckEnable()
{
	if (myOwner == nullptr)
		return; 

	if (Cast<AComponentRPG_PlayerController>(myController)->ControllerIndex != Cast<AComponentRPGCharacter>(myOwner)->TeamIndex || Cast<AComponentRPGCharacter>(myOwner)->IsCastingSkill 
		|| (Cast<AComponentRPGCharacter>(myOwner)->IsMovingToLocation && GetPercentActionBar() < 1.0f))
		SkillDeckHUD_UI->SetIsEnabled(false);
	else
		SkillDeckHUD_UI->SetIsEnabled(true);
}