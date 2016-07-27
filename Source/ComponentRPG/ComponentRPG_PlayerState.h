// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ComponentRPG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API AComponentRPG_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public: 

	int32 PlayerIndex;
	
	
};
