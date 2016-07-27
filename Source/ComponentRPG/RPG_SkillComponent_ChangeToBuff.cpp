// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_ChangeToBuff.h"


URPG_SkillComponent_ChangeToBuff::URPG_SkillComponent_ChangeToBuff(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_ChangeToBuff::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_ChangeToBuff;
	
	BuffDuration = 5.0f;
	bIsGroundBuff = false;
}

void URPG_SkillComponent_ChangeToBuff::ComponentSetup(FSkillComponentCustomization custom)
{
	// int1 = is on ground persist buff?
	// float1 = buff duration

	if (custom.Customize_int1 == 1)
		bIsGroundBuff = true;

	if (custom.Customize_float1 > 0)
		BuffDuration = custom.Customize_float1;
}

void URPG_SkillComponent_ChangeToBuff::BuffActorModification(AComponentRPG_BuffActor* buff_actor)
{
	buff_actor->bIsInstantBuff = false;
	buff_actor->SetActorLifeSpan(BuffDuration);
}

void URPG_SkillComponent_ChangeToBuff::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_ChangeToBuff, bIsGroundBuff);
	DOREPLIFETIME(URPG_SkillComponent_ChangeToBuff, BuffDuration);
}

FString URPG_SkillComponent_ChangeToBuff::GetSkillComponentDescription()
{
	return "Change to Buff";
}

FString URPG_SkillComponent_ChangeToBuff::GetSkillComponentDescriptionTooltip()
{
	FString result = "";
	result += GetSkillComponentBasicTooltipString();
	result += "Change to Buff";
	result += "\n-" + FString::Printf(TEXT("%.2f"), BuffDuration) + " s";
	result += (bIsGroundBuff) ? "\n-Ground AOE" : "";

	return result;
}
