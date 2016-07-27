// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_BuffActor.h"
#include "BuffArrayByType.h"


bool UBuffArrayByType::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = false;

	//for (AComponentRPG_BuffActor * myObject : BuffArray)
	//{
	//	if (myObject != nullptr)
	//	{
	//		WroteSomething |= Channel->ReplicateSubobject(myObject, *Bunch, *RepFlags);
	//	}
	//}

	return WroteSomething;
}

void UBuffArrayByType::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UBuffArrayByType, BuffArray);
}

