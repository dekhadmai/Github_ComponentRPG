// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_PlusNumTarget.h"

URPG_SkillComponent_PlusNumTarget::URPG_SkillComponent_PlusNumTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_PlusNumTarget::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_AdditionalTarget;

	AdditionalNumTarget = 1;
}

void URPG_SkillComponent_PlusNumTarget::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_PlusNumTarget, AdditionalNumTarget);
}

FString URPG_SkillComponent_PlusNumTarget::GetSkillComponentDescription()
{
	return "+" + FString::Printf(TEXT("%d"), AdditionalNumTarget) + " Target";
}

FString URPG_SkillComponent_PlusNumTarget::GetSkillComponentDescriptionTooltip()
{
	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	result += "+" + FString::Printf(TEXT("%d"), AdditionalNumTarget) + " Target";

	return result;
}


