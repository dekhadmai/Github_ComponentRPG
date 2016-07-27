// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPG_TargetingWidget.h"


void UComponentRPG_TargetingWidget::SetTargetingType(TargetingType val)
{
	if (myController != nullptr)
		Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_SetTargetingType(SkillComponent, val);
}

void UComponentRPG_TargetingWidget::SetTargetSideType(TargetSideType val)
{
	if (myController != nullptr)
		Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_SetTargetSideType(SkillComponent, val);
}

void UComponentRPG_TargetingWidget::SetTargetType(TargetType val)
{
	if (myController != nullptr)
		Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_SetTargetType(SkillComponent, val);
}

ECheckBoxState UComponentRPG_TargetingWidget::GetTargetingType(TargetingType val)
{
	if (SkillComponent->GetTargetingType() == val)
		return ECheckBoxState::Checked;
	else
		return ECheckBoxState::Unchecked;
}

ECheckBoxState UComponentRPG_TargetingWidget::GetTargetSideType(TargetSideType val)
{
	if (SkillComponent->GetTargetSideType() == val)
		return ECheckBoxState::Checked;
	else
		return ECheckBoxState::Unchecked;
}

ECheckBoxState UComponentRPG_TargetingWidget::GetTargetType(TargetType val)
{
	if (SkillComponent->GetTargetType() == val)
		return ECheckBoxState::Checked;
	else
		return ECheckBoxState::Unchecked;
}

ECheckBoxState UComponentRPG_TargetingWidget::GetIsSelectingTarget()
{
	if (SkillComponent->GetIsSelectingTarget())
		return ECheckBoxState::Checked;
	else
		return ECheckBoxState::Unchecked;
}

void UComponentRPG_TargetingWidget::SelectTargetButtonClicked()
{
	if (!SkillComponent->GetIsSelectingTarget())
		SetIsSelectingTarget(true);
	else
		SetIsSelectingTarget(false);
}

void UComponentRPG_TargetingWidget::SetIsSelectingTarget(bool val)
{
	if (val)
	{
		/*if (myController != nullptr)
			Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_StartSelectingTarget(SkillComponent);*/
		SkillComponent->StartSelectingTarget();
	}
	else
	{
		/*if (myController != nullptr)
			Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_StopSelectingTarget(SkillComponent);*/
		SkillComponent->StopSelectingTarget();
	}
}

bool UComponentRPG_TargetingWidget::GetIsCharacterButtonEnable()
{
	if (SkillComponent->GetTargetingType() == TargetingType::TargetingType_AOE)
	{
		if (SkillComponent->GetTargetType() != TargetType::TargetType_Ground)
			SetTargetType(TargetType::TargetType_Ground);

		return false;
	}
	else
	{
		return true;
	}
}