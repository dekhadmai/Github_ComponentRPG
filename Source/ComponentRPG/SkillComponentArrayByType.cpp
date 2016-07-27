// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_SkillComponent.h"
#include "SkillComponentArrayByType.h"


bool USkillComponentArrayByType::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = false;

	for (UComponentRPG_SkillComponent * myObject : SkillComponentArray)
	{
		if (myObject != nullptr)
		{
			WroteSomething |= Channel->ReplicateSubobject(myObject, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USkillComponentArrayByType::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(USkillComponentArrayByType, SkillComponentArray);
}

