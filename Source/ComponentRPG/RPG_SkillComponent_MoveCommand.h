// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPG_SkillComponent_Targeting.h"
#include "RPG_SkillComponent_MoveCommand.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_MoveCommand : public URPG_SkillComponent_Targeting
{
	GENERATED_BODY()
	
	URPG_SkillComponent_MoveCommand(const FObjectInitializer& ObjectInitializer);

public :
	virtual void Init(void* obj = NULL) override;
	//virtual FString GetSkillComponentDescription() override;
	virtual void ResetDefaultValue() override;

	//virtual void StartSelectingTarget() override;
	//virtual void StopSelectingTarget() override;

	virtual void SetClientClearTargetingComponent() override;

	virtual int32 GetNumTarget() override;
	virtual float GetAreaSize() override;

	//virtual void SetIsSelected(bool val);
};
