// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_Damage.h"

URPG_SkillComponent_Damage::URPG_SkillComponent_Damage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_Damage::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_Damage;

	DamageAmount = 30.0f;
}

void URPG_SkillComponent_Damage::BuffActorModification(AComponentRPG_BuffActor* buff_actor)
{
	buff_actor->BuffDamage += DamageAmount;
}

void URPG_SkillComponent_Damage::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_Damage, DamageAmount);
}

FString URPG_SkillComponent_Damage::GetSkillComponentDescription()
{
	return "+Base Damage";
}

FString URPG_SkillComponent_Damage::GetSkillComponentDescriptionTooltip()
{
	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	result += "+" + FString::Printf(TEXT("%.2f"), DamageAmount) + " Damage";

	return result;
}