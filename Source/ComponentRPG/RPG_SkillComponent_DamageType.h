// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_DamageType.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_DamageType : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()

public:
	URPG_SkillComponent_DamageType(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		DamageType DmgType;
	
	virtual void Init(void* obj = NULL) override;
	virtual void ComponentSetup(FSkillComponentCustomization custom) override;
	virtual void BuffActorModification(AComponentRPG_BuffActor* buff_actor) override;

	virtual FString GetSkillComponentDescription() override;
	virtual FString GetSkillComponentDescriptionTooltip() override;	

	//virtual void Activate() override;
	//virtual void OnTickEffect(float deltaTime) override;
	//virtual void Deactivate() override; 
};
