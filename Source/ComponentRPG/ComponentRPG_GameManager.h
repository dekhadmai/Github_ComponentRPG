// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_GameManager.generated.h"

class AComponentRPG_PlayerController;
/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_GameManager : public UObject
{
	GENERATED_BODY()
	
	UComponentRPG_GameManager(const FObjectInitializer& ObjectInitializer);
	
//public:
//	UPROPERTY()
//	ACameraActor* MainCamera;
//
//	UPROPERTY()
//	AComponentRPG_PlayerController* myController;
//
//	UPROPERTY()
//	FVector BoardPosition;
//
//	UPROPERTY()
//	FRotator BoardRotation;
//
//	UPROPERTY()
//	float TileSize;
//
//	UPROPERTY()
//	TArray<AComponentRPGCharacter*> CharacterArray;
//
//	// 0,0 is the top left
//	// 8,2 is the bottom right
//	FVector GetTileCenterPosition(int32 x, int32 z);
//
//	FVector GetSpawnPosition(int32 areaIndex);
//
//	int32 SetupCount;
//	void GameManagerInit();
};
