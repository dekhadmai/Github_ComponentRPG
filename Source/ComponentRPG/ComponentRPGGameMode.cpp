// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ComponentRPG.h"
#include "ComponentRPGGameMode.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_HUD.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPG_PlayerState.h"
#include "ComponentRPG_GameState.h"

AComponentRPGGameMode::AComponentRPGGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = AComponentRPGCharacter::StaticClass();	
	PlayerControllerClass = AComponentRPG_PlayerController::StaticClass();
	HUDClass = AComponentRPG_HUD::StaticClass();
	GameStateClass = AComponentRPG_GameState::StaticClass();
	
	//// Create an orthographic camera (no perspective) and attach it to the boom
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	//SideViewCameraComponent->OrthoWidth = 2048.0f;
	//SideViewCameraComponent->SetWorldLocation(FVector(-30, 570, 530));
	
}

void AComponentRPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Cast<AComponentRPG_PlayerController>(NewPlayer)->ControllerIndex = NumPlayers - 1;
	Cast<AComponentRPG_PlayerController>(NewPlayer)->SetControllerIndex(NumPlayers-1);

	//FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPGGameMode::SetupGameStateInit, 0.5f, false);
}

void AComponentRPGGameMode::SetupGameStateInit()
{
	//Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->GameStateInit();
}

void AComponentRPGGameMode::BeginPlay()
{
	Super::BeginPlay();
    ChangeMenuWidget(StartingWidgetClass);
}

void AComponentRPGGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
		CurrentWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}