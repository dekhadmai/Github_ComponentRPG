// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_DamageType.h"
#include "RPG_SkillComponent_Damage.h"
#include "RPG_SkillComponent_Targeting.h"
#include "RPG_SkillComponent_PlusNumTarget.h"
#include "RPG_SkillComponent_PlusAreaSize.h"
#include "RPG_SkillComponent_ChangeToBuff.h"
#include "RPG_Skillcomponent_ChanceOnHit.h"
#include "RPG_SkillComponent_MoveCommand.h"
#include "RPG_SkillComponent_MoveOrder.h"
#include "RPG_SkillComponent_Execute.h"
#include "ComponentRPG_SkillComponentDeck.generated.h"

/**
 * 
 */
class AComponentRPG_BaseProjectile;
class AComponentRPGCharacter;

UCLASS()
class COMPONENTRPG_API UComponentRPG_SkillComponentDeck : public UObject
{
	GENERATED_BODY()
	
public: 

	///// DeckOwner = AComponentRPGCharacter
	UPROPERTY(Replicated)
		UObject* DeckOwner;

	UComponentRPG_SkillComponentDeck(const FObjectInitializer& ObjectInitializer);
	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);

	void SetDeckOwner(UObject* owner);
	void Init();
	void AddSkillComponentToDeck(UComponentRPG_SkillComponent* comp);
	UComponentRPG_SkillComponent* GetTargetingComponent();
	UComponentRPG_SkillComponent* GetMoveCommandComponent();

	UPROPERTY(Replicated)
		TArray<UComponentRPG_SkillComponent*> SkillComponentArray;

	UPROPERTY(Replicated)
		TArray<USkillComponentArrayByType*> SkillComponentArrayByType;

	/////
	void ExecuteAction();
	void FinishCastingSkill();

	///// Helper function
	int32 GetSumAdditionalNumTarget();
	float GetSumAdditionalAreaSize();
	float GetTotalCastRateModifier();

	void ProjectileCommonSetup(AComponentRPG_BaseProjectile* tmp_proj, AComponentRPGCharacter* tmp_char);
};
