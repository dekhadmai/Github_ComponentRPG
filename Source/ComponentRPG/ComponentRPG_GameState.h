// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Http.h"
#include "Json.h"
#include "GameFramework/GameState.h"
#include "ComponentRPG_GameManager.h"
#include "ComponentRPG_BaseActor.h"
#include "ComponentRPG_BaseProjectile.h"
#include "ComponentRPG_BuffActor.h"
#include "ComponentRPG_GameState.generated.h"

/**
 * 
 */
class UComponentRPG_SaveProfile;

UCLASS()
class COMPONENTRPG_API AComponentRPG_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AComponentRPG_GameState(const FObjectInitializer& ObjectInitializer);

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;

	UPROPERTY()
	UComponentRPG_GameManager* GameManager;

	UPROPERTY()
		AComponentRPG_BaseActor* baseactor;

	UPROPERTY()
		ACameraActor* MainCamera;

	UPROPERTY()
		AComponentRPG_PlayerController* myController;

	UPROPERTY()
		FVector BoardPosition;

	UPROPERTY()
		FRotator BoardRotation;

	UPROPERTY()
		float TileSize;

	UPROPERTY()
		TArray<AComponentRPGCharacter*> CharacterArray;

	UPROPERTY()
		TArray<AActor*> NonCharacterArray;

	UPROPERTY(Replicated)
		AComponentRPGCharacter* CurrentCharacterTurn;
	UPROPERTY(Replicated)
		float CurrentCharacterStartTurnTime;
	UPROPERTY()
		FTimerHandle TurnTimerHandle;

	void EndTurnTimer();

	// 0,0 is the top left
	// 8,2 is the bottom right
	FVector GetTileCenterPosition(int32 x, int32 z);

	FVector GetSpawnPosition(int32 areaIndex);

	int32 SetupCount;
	void GameStateInit(int player_index, UComponentRPG_SaveProfile* save_profile);
	AComponentRPG_BaseProjectile* SpawnProjectileActor(FVector SpawnLoc, FRotator SpawnRot, APawn* proj_Owner, bool isAreaBuff);
	AComponentRPG_BuffActor* SpawnBuffActor();

	void WaitForCharacterTurn(AComponentRPGCharacter* current_char = nullptr);
	void CharacterExecuteAction(AComponentRPGCharacter* current_char = nullptr);

	void SetGlobalTimeDilation(float val);

	///// Component List
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TArray<UClass*> AllSkillComponentList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TArray<UClass*> MainSkillComponentList;

	///// HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> CustomTooltipWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> PlayerHUD_UI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> PlayerController_UI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> SkillDeck_UI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> TranslucentHUD_UI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<class AComponentRPGCharacter> CharacterBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<class AComponentRPG_BaseProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<class AComponentRPG_BaseProjectile> AreaBuffBlueprint;



	/////////////////////////////////////////////////////
	////////// Http land, game code end here ///////////
	///////////////////////////////////////////////////

	int LastControllerRequest;
	void HttpRequest_LoadPlayerProfile(FString url, int controller_index);
	void HttpResponse_LoadPlayerProfile(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//void HttpRequest();
	//void HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
