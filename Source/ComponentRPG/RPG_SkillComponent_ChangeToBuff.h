// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_ChangeToBuff.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_ChangeToBuff : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()
	
public: 
	URPG_SkillComponent_ChangeToBuff(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		float BuffDuration;

	UPROPERTY(Replicated)
		bool bIsGroundBuff;

	virtual void Init(void* obj = NULL) override;
	virtual void ComponentSetup(FSkillComponentCustomization custom) override;

	virtual void BuffActorModification(AComponentRPG_BuffActor* buff_actor) override;

	virtual FString GetSkillComponentDescription() override;	
	virtual FString GetSkillComponentDescriptionTooltip() override;	
	
};
