// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPGCharacter.h"
#include "RPG_SkillComponent_Targeting.h"
#include "RPG_SkillComponent_Execute.h"

URPG_SkillComponent_Execute::URPG_SkillComponent_Execute(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_Execute::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_Execute;
}

void URPG_SkillComponent_Execute::SetIsSelected(bool val)
{
	if (val)
		Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->ExecuteAction();
}

bool URPG_SkillComponent_Execute::GetIsEnabled()
{
	if (ComponentOwner == nullptr)
		return false;

	return (Cast<AComponentRPGCharacter>(ComponentOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR / Cast<AComponentRPGCharacter>(ComponentOwner)->CharStatComponent->BaseStats.BASE_ACTIONBAR) >= 1.0f
		&& !Cast<AComponentRPGCharacter>(ComponentOwner)->IsCastingSkill && Cast<AComponentRPGCharacter>(ComponentOwner)->bWaitingForAction;
}

FString URPG_SkillComponent_Execute::GetSkillComponentDescription()
{
	if (Cast<AComponentRPGCharacter>(ComponentOwner) == nullptr)
		return "";

	//bool CheckMoveComp = Cast<URPG_SkillComponent_Targeting>(Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetMoveCommandComponent())->TargetLocationArray.Num() > 0;
	bool CheckMoveComp = Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetMoveCommandComponent()->GetIsSelected();
	bool CheckTargetingComp = (Cast<URPG_SkillComponent_Targeting>(Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetTargetingComponent())->TargetLocationArray.Num() +
		Cast<URPG_SkillComponent_Targeting>(Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetTargetingComponent())->TargetCharacterArray.Num()) > 0;
	
	if (CheckMoveComp)
		return "Action - Movement";
	else if (CheckTargetingComp)
		return "Action - Cast Skill";
	else
		return "Skip Turn";
}

FString URPG_SkillComponent_Execute::GetSkillComponentDescriptionTooltip()
{
	return GetSkillComponentDescription();
}


