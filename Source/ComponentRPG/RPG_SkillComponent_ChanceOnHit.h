// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_ChanceOnHit.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_ChanceOnHit : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()
	
public: 
	URPG_SkillComponent_ChanceOnHit(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		FChanceOnHitStruct ChanceStruct;

	virtual void Init(void* obj = NULL) override;
	virtual void ComponentSetup(FSkillComponentCustomization custom) override;
	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;		
	
};
