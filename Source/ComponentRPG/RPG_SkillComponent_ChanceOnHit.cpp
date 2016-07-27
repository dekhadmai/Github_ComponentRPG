// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_ChanceOnHit.h"


URPG_SkillComponent_ChanceOnHit::URPG_SkillComponent_ChanceOnHit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_ChanceOnHit::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_ChanceOnHit;
}

void URPG_SkillComponent_ChanceOnHit::ComponentSetup(FSkillComponentCustomization custom)
{
	// int1 = chance on hit - effect type
	// float1 = percent chance, -1 mean infinite/always apply ailment
	// float2 = effect duration

	if (custom.Customize_int1 > 0)
		ChanceStruct.Effect = (ChanceOnHitEffect)(custom.Customize_int1);

	if (custom.Customize_float1 != 0)
		ChanceStruct.PercentChance = custom.Customize_float1;

	if (custom.Customize_float2 != 0)
		ChanceStruct.EffectDuration = custom.Customize_float2;
}

void URPG_SkillComponent_ChanceOnHit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_ChanceOnHit, ChanceStruct);
}

FString URPG_SkillComponent_ChanceOnHit::GetSkillComponentDescription()
{
	return "Chance on hit";
}

FString URPG_SkillComponent_ChanceOnHit::GetSkillComponentDescriptionTooltip()
{
	FString strType = "";

	switch (ChanceStruct.Effect)
	{
		case ChanceOnHitEffect::ChanceOnHitEffect_Freeze : 
			strType = "Freeze"; break;
		case ChanceOnHitEffect::ChanceOnHitEffect_Burn : 
			strType = "Burn"; break;
		case ChanceOnHitEffect::ChanceOnHitEffect_Stun : 
			strType = "Stun"; break;
		case ChanceOnHitEffect::ChanceOnHitEffect_Critical : 
			strType = "Critical"; break;
		case ChanceOnHitEffect::ChanceOnHitEffect_Interrupt : 
			strType = "Interrupt"; break;
		default : break;
	}

	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	if (ChanceStruct.PercentChance != -1)
		result += FString::Printf(TEXT("%.2f"), ChanceStruct.PercentChance * 100) + "% chance to ";
	else
		result += "Always ";

	result += strType + " on hit ";

	if (ChanceStruct.EffectDuration != -1)
		result += "for " + FString::Printf(TEXT("%.2f"), ChanceStruct.EffectDuration) + " s";

	return result;
}