// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BuffArrayByType.generated.h"

class AComponentRPG_BuffActor;
/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UBuffArrayByType : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);

	UPROPERTY(Replicated)
		TArray<AComponentRPG_BuffActor*> BuffArray;
};