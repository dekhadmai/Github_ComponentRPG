// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "RPG_SkillComponent_MoveCommand.h"
#include "ComponentRPGCharacter.h"

URPG_SkillComponent_MoveCommand::URPG_SkillComponent_MoveCommand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_MoveCommand::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_MoveCommand;
	SetIsSelected(true);
	IndicatorColor = FLinearColor(0.0f, 0.0f, 0.5f, 0.5f);
}

void URPG_SkillComponent_MoveCommand::ResetDefaultValue()
{
	targeting_instruction.num_of_target = 1;
	targeting_instruction.area_size = 0.2f;

	targeting_instruction.targeting_type = TargetingType::TargetingType_Missile;
	targeting_instruction.target_side = TargetSideType::TargetSideType_Both;
	targeting_instruction.target_type = TargetType::TargetType_Ground;
}

//void URPG_SkillComponent_MoveCommand::StartSelectingTarget()
//{
//	Super::StartSelectingTarget();
//	SetIsSelected(true);
//}
//
//void URPG_SkillComponent_MoveCommand::StopSelectingTarget()
//{
//	Super::StopSelectingTarget();
//	SetIsSelected(true);
//}

void URPG_SkillComponent_MoveCommand::SetClientClearTargetingComponent()
{
	//Cast<AComponentRPGCharacter>(ComponentOwner)->SetClientClearMovementComponent(true);
}

int32 URPG_SkillComponent_MoveCommand::GetNumTarget()
{
	return targeting_instruction.num_of_target;
}

float URPG_SkillComponent_MoveCommand::GetAreaSize()
{
	return targeting_instruction.area_size;
}