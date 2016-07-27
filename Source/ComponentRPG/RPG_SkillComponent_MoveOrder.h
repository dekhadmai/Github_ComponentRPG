// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_MoveOrder.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_MoveOrder : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()

public:
	URPG_SkillComponent_MoveOrder(const FObjectInitializer& ObjectInitializer);

	virtual void Init(void* obj = NULL) override;
	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;

	//virtual void SetIsSelected(bool val) override;
	virtual bool GetIsEnabled() override;
	
};
