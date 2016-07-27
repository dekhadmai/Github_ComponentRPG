// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_PlusNumTarget.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_PlusNumTarget : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()
		URPG_SkillComponent_PlusNumTarget(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(Replicated)
		int32 AdditionalNumTarget;

	virtual void Init(void* obj = NULL) override;
	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;
};
