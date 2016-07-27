// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPG_SkillComponent.h"
#include "ComponentRPG_BuffActor.h"
#include "BuffArrayByType.h"
#include "ComponentRPG_BuffDeck.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_BuffDeck : public UObject
{
	GENERATED_BODY()
	
public: 

	///// DeckOwner = AComponentRPGCharacter
	UPROPERTY(Replicated)
		AActor* DeckOwner;

	UComponentRPG_BuffDeck(const FObjectInitializer& ObjectInitializer);
	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);

	void SetDeckOwner(AActor* owner);
	void Init();
	void AddBuffToDeck(AComponentRPG_BuffActor* buff);
	//UComponentRPG_SkillComponent* GetTargetingComponent();
	//UComponentRPG_SkillComponent* GetMoveCommandComponent();

	UPROPERTY(Replicated)
		TArray<AComponentRPG_BuffActor*> BuffArray;

	UPROPERTY(Replicated)
		TArray<UBuffArrayByType*> BuffArrayByType;

	///// Helper function
	//int32 GetSumAdditionalNumTarget();
	//float GetSumAdditionalAreaSize();
	
	
};
