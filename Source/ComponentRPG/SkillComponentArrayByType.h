// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SkillComponentArrayByType.generated.h"

class UComponentRPG_SkillComponent;
/**
 * 
 */
UCLASS()
class COMPONENTRPG_API USkillComponentArrayByType : public UObject
{
	GENERATED_BODY()
	

public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);

	UPROPERTY(Replicated)
		TArray<UComponentRPG_SkillComponent*> SkillComponentArray;
};
