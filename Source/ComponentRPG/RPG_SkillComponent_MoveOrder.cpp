// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_MoveOrder.h"


URPG_SkillComponent_MoveOrder::URPG_SkillComponent_MoveOrder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_MoveOrder::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_MoveCommand;
	
	CastRateModifier = 1.0f;
}

//void URPG_SkillComponent_MoveOrder::SetIsSelected(bool val)
//{
//	if (val)
//		Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->ExecuteAction();
//}

bool URPG_SkillComponent_MoveOrder::GetIsEnabled()
{
	if (ComponentOwner == nullptr)
		return false;

	return (Cast<AComponentRPGCharacter>(ComponentOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR / Cast<AComponentRPGCharacter>(ComponentOwner)->CharStatComponent->BaseStats.BASE_ACTIONBAR) >= 1.0f
		&& !Cast<AComponentRPGCharacter>(ComponentOwner)->IsCastingSkill && Cast<AComponentRPGCharacter>(ComponentOwner)->bWaitingForAction;
}

FString URPG_SkillComponent_MoveOrder::GetSkillComponentDescription()
{
	return "Move";
}

FString URPG_SkillComponent_MoveOrder::GetSkillComponentDescriptionTooltip()
{
	return "Move to target location";
}

