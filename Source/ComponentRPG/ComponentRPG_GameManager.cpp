// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_GameManager.h"
#include "ComponentRPG_PlayerController.h"

UComponentRPG_GameManager::UComponentRPG_GameManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*TileSize = 32.0f * 4.0f;
	SetupCount = 0;*/
}

//void UComponentRPG_GameManager::GameManagerInit()
//{
//	///// init characters
//	// spawn player 1 x=0-2, z=0-4
//
//	APawn* oriChar = myController->GetPawn();
//	for (int32 i = 0; i < 3; ++i)
//	{
//		FVector SpawnLoc = GetSpawnPosition(SetupCount);
//		CharacterArray.Add( myController->GetWorld()->SpawnActor<AComponentRPGCharacter>(SpawnLoc, BoardRotation) );
//	}
//
//	SetupCount++;
//		
//	/*myController->Possess( CharacterArray[0] );
//	oriChar->Destroy();*/
//}
//
//FVector UComponentRPG_GameManager::GetTileCenterPosition(int32 x, int32 z)
//{
//	// 0,0 is the top left
//	// 8,2 is the bottom right
//	FVector result;
//
//	result.X = BoardPosition.X + (TileSize * x);
//	result.Z = BoardPosition.Z - (TileSize * z);
//	result.Y = BoardPosition.Y + 1.0f;
//
//	return result;
//}
//
//FVector UComponentRPG_GameManager::GetSpawnPosition(int32 areaIndex)
//{
//	int32 randMinX = 0 + ((areaIndex == 0) ? 0 : 6);
//	int32 randMaxX = 2 + ((areaIndex == 0) ? 0 : 6);
//	int32 randMinY = 0;
//	int32 randMaxY = 4;
//
//	FVector SpawnLoc = GetTileCenterPosition(FMath::RandRange(randMinX,randMaxX),FMath::RandRange(randMinY,randMaxY));
//	for (int32 i = 0; i < CharacterArray.Num(); ++i)
//	{
//		if (CharacterArray[i]->GetActorLocation() == SpawnLoc)
//		{
//			// same pos, repick the cell
//			SpawnLoc = GetSpawnPosition(areaIndex);
//			break;
//		}
//	}
//	
//	return SpawnLoc;
//}