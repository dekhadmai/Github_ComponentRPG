// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_Damage.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_Damage : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()

public: 
	URPG_SkillComponent_Damage(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		float DamageAmount;

	virtual void Init(void* obj = NULL) override;
	virtual void BuffActorModification(AComponentRPG_BuffActor* buff_actor) override;

	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;	
};