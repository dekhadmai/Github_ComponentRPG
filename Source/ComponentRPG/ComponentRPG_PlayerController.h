// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Http.h"
#include "Json.h"
#include "ComponentRPG_GameState.h"
#include "GameFramework/PlayerController.h"
#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_SkillComponent.h"
#include "ComponentRPG_SaveProfile.h"
#include "ComponentRPG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API AComponentRPG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AComponentRPG_PlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;
	virtual void BeginPlayingState() override;

	void PostBeginPlay();
	
	// controller things
	/*void Jump();
	void StopJumping();*/
	void MoveRight(float Value);


	void MouseClick();

	///// ComponentRPG stuff
	bool isInitOnce;

	void SetViewTargetTimer();

public: 

	UFUNCTION(reliable, server, WithValidation)
		void CallServerPossess(APawn* InPawn);

	UFUNCTION(reliable, server, WithValidation)
		void CallServerSetMoveToLocationCommand(APawn* InPawn, FVector move_location);

	UFUNCTION(Reliable, Client)
		void ClientChangeWidgetOwner(APawn* InPawn);

	void ChangeWidgetOwner(APawn* InPawn);

	virtual void Possess(APawn* InPawn) override;

	/*UPROPERTY()
	AComponentRPG_GameState* myGameState;*/

	UFUNCTION(Reliable, Client)
		void SetControllerIndex(uint32 index);

	UFUNCTION(Reliable, Client)
		void AddWidgetOnController(TSubclassOf<UComponentRPG_UserWidget> widget_template, UObject* widget_owner);

	UFUNCTION(Reliable, Client)
		void ClientCreateHUDWidget();

	UFUNCTION(Reliable, Client)
		void ClientCreateHUD_StartCombat();

	void CreateHUDWidgetTimer();
	void CreateHUDWidget();

	int32 ControllerIndex;

	///// HUD-Related
	//void SetIsMoveCommand();

	///// SkillComponent RPC
	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_SetIsSelected(UComponentRPG_SkillComponent* skill_component, bool val);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_SetTargetingType(UComponentRPG_SkillComponent* skill_component, TargetingType val);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_SetTargetSideType(UComponentRPG_SkillComponent* skill_component, TargetSideType val);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_SetTargetType(UComponentRPG_SkillComponent* skill_component, TargetType val);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_StartSelectingTarget(UComponentRPG_SkillComponent* skill_component);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_StopSelectingTarget(UComponentRPG_SkillComponent* skill_component);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_ClearAllTarget(UComponentRPG_SkillComponent* skill_component);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_AddTargetCharacterToArray(UComponentRPG_SkillComponent* skill_component, ACharacter* targetChar);

	UFUNCTION(Reliable, Server, WithValidation)
		void SkillComponent_AddTargetLocationToArray(UComponentRPG_SkillComponent* skill_component, FVector targetLoc);


	///// HUD
    /** The widget instance that we are using as our menu. */
	UPROPERTY()
		UComponentRPG_UserWidget* CurrentWidget;

	UPROPERTY()
		AComponentRPG_BaseProjectile* InspectedProjectile;

	///// HTTP land
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_Http_LoadProfile(const FString& username);

	void HttpRequest_LoadPlayerProfile(FString username);
	void HttpResponse_LoadPlayerProfile(UComponentRPG_SaveProfile* save_profile);
	///// end of HTTP land


	///// Persist SaveProfile on PlayerController
	UPROPERTY()
		UComponentRPG_SaveProfile* mySaveProfile;

	///// Menu widget UI (non-gameplay widget) /////

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected: 
	/** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> MainMenuWidgetClass; // start, option

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> LoginWidgetClass; // start > username, host/join

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
		UComponentRPG_UserWidget* NonGameplay_CurrentWidget;
};
