// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_PlusAreaSize.h"

URPG_SkillComponent_PlusAreaSize::URPG_SkillComponent_PlusAreaSize(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_PlusAreaSize::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_AdditionalAreaSize;

	AdditionalAreaSize = 0.5f;
}

void URPG_SkillComponent_PlusAreaSize::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_PlusAreaSize, AdditionalAreaSize);
}

FString URPG_SkillComponent_PlusAreaSize::GetSkillComponentDescription()
{
	return "+" + FString::Printf(TEXT("%.2f"), AdditionalAreaSize) + " AOE";
}

FString URPG_SkillComponent_PlusAreaSize::GetSkillComponentDescriptionTooltip()
{
	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	result += "+" + FString::Printf(TEXT("%.2f"), AdditionalAreaSize) + " AOE";
	
	return result;
}


