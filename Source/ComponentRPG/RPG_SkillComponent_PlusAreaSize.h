// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_PlusAreaSize.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_PlusAreaSize : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()
		URPG_SkillComponent_PlusAreaSize(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(Replicated)
		float AdditionalAreaSize;

	virtual void Init(void* obj = NULL) override;
	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;
};
