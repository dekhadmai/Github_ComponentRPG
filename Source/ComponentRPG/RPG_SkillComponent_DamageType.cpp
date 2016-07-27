// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_SaveProfile.h"
#include "RPG_SkillComponent_DamageType.h"

URPG_SkillComponent_DamageType::URPG_SkillComponent_DamageType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_DamageType::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_DamageType;

	DmgType = DamageType::DamageType_NoElement;
}

void URPG_SkillComponent_DamageType::ComponentSetup(FSkillComponentCustomization custom)
{
	// int1 = damage type

	if (custom.Customize_int1 > 0)
		DmgType = (DamageType)(custom.Customize_int1);
}

void URPG_SkillComponent_DamageType::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_DamageType, DmgType);
}

void URPG_SkillComponent_DamageType::BuffActorModification(AComponentRPG_BuffActor* buff_actor)
{
	buff_actor->BuffDamageType = DmgType;
}

FString URPG_SkillComponent_DamageType::GetSkillComponentDescription()
{
	return "Change Damage Type";
}

FString URPG_SkillComponent_DamageType::GetSkillComponentDescriptionTooltip()
{
	FString strType = "";

	switch (DmgType)
	{
		case DamageType::DamageType_NoElement : 
			strType = "NonElement"; break;
		case DamageType::DamageType_Fire : 
			strType = "Fire"; break;
		case DamageType::DamageType_Ice : 
			strType = "Ice"; break;
		case DamageType::DamageType_Lightning : 
			strType = "Lightning"; break;
		case DamageType::DamageType_Poison : 
			strType = "Poison"; break;
		case DamageType::DamageType_Heal : 
			strType = "Heal"; break;
		default : break;
	}

	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	result += strType + " Damage";

	return result;
}