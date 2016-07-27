// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/GameMode.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_GameManager.h"
#include "ComponentRPGGameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of ComponentRPGCharacter

UCLASS(minimalapi)
class AComponentRPGGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes, meta=(DisplayName="HUD Class"))
	TSubclassOf<class AHUD> HUDClass;    */

	AComponentRPGGameMode();

public: 
	virtual void BeginPlay() override;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	virtual void PostLogin(APlayerController* NewPlayer);
	void SetupGameStateInit();

protected: 
	/** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UComponentRPG_UserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
	UComponentRPG_UserWidget* CurrentWidget;
};
