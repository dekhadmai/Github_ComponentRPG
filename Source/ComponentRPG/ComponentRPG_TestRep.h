// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPG_SkillComponentDeck.h"
#include "ComponentRPG_BuffActor.h"
#include "ComponentRPG_TestRep.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_TestRep : public UObject
{
	GENERATED_BODY()
	
public:
	UComponentRPG_TestRep(const FObjectInitializer& ObjectInitializer);

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);
	
	UPROPERTY(Replicated)
		ChanceOnHitEffect testEnum;

	UPROPERTY(Replicated)
		AComponentRPG_BuffActor* testComp;

	UPROPERTY(Replicated)
		TArray<AComponentRPG_BuffActor*> testCompArray;

	void Init();
	
	UFUNCTION()
		void AddBuffActor(AComponentRPG_BuffActor* buff);
	
	///////
	//UPROPERTY()
	//	URPG_SkillComponent_Targeting* tmp1;

	//UPROPERTY()
	//	URPG_SkillComponent_Damage* tmp2;

	//UPROPERTY()
	//	URPG_SkillComponent_DamageType* tmp3;

	//UPROPERTY()
	//	URPG_SkillComponent_PlusNumTarget* tmp4;

	//UPROPERTY()
	//	URPG_SkillComponent_PlusNumTarget* tmp5;

	//UPROPERTY()
	//	URPG_SkillComponent_PlusAreaSize* tmp6;

	//UPROPERTY()
	//	URPG_SkillComponent_PlusAreaSize* tmp7;
};
