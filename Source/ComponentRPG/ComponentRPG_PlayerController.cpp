// Fill out your copyright notice in the Description page of Project Settings.

//#include "DrawDebugHelpers.h"
#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_PlayerControllerHUD.h"
#include "ComponentRPG_SkillDeckHUD.h"

AComponentRPG_PlayerController::AComponentRPG_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	bReplicates = true;

	isInitOnce = false;
}

//void AComponentRPG_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	DOREPLIFETIME(AComponentRPG_PlayerController, mySaveProfile);
//}

void AComponentRPG_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*InputComponent->BindAction("Jump", IE_Pressed, this, &AComponentRPG_PlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AComponentRPG_PlayerController::StopJumping);*/

	InputComponent->BindAxis("MoveRight", this, &AComponentRPG_PlayerController::MoveRight);

	InputComponent->BindAction("MouseClick", IE_Pressed, this, &AComponentRPG_PlayerController::MouseClick);
	//InputComponent->BindAction("MouseClick", IE_Released, this, &AComponentRPG_PlayerController::MouseClick);

	//InputComponent->BindAction("HttpRequest", IE_Released, this, &AComponentRPG_PlayerController::HttpRequest_LoadPlayerProfile);
}

void AComponentRPG_PlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	bAutoManageActiveCameraTarget = false;

	PostBeginPlay();
}

void AComponentRPG_PlayerController::PostBeginPlay()
{
	if (!isInitOnce)
	{
		ClientCreateHUDWidget();

		// destroy original character
		GetCharacter()->Destroy();

		isInitOnce = true;
	}
}

void AComponentRPG_PlayerController::SetViewTargetTimer()
{
	AComponentRPG_GameState* myGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (myGameState == nullptr || myGameState->MainCamera == nullptr)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPG_PlayerController::SetViewTargetTimer, 0.2f, false);

		return;
	}
	
	SetViewTarget(myGameState->MainCamera);
}


void AComponentRPG_PlayerController::ClientCreateHUDWidget_Implementation()
{
	CreateHUDWidgetTimer();
}

void AComponentRPG_PlayerController::CreateHUDWidgetTimer()
{
	AComponentRPG_GameState* myGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (myGameState == nullptr)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPG_PlayerController::CreateHUDWidgetTimer, 0.2f, false);

		return;
	}

	CreateHUDWidget();
}

void AComponentRPG_PlayerController::CreateHUDWidget()
{
	if (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
	{
		ChangeMenuWidget(MainMenuWidgetClass);
	}
	else
	{
		if (GEngine->GetNetMode(GetWorld()) != NM_DedicatedServer)
			ChangeMenuWidget(LoginWidgetClass);
	}

	SetViewTargetTimer();
}

//void AComponentRPG_PlayerController::Jump()
//{
//	Cast<AComponentRPGCharacter>(GetCharacter())->Jump();
//}
//void AComponentRPG_PlayerController::StopJumping()
//{
//	Cast<AComponentRPGCharacter>(GetCharacter())->StopJumping();
//}
void AComponentRPG_PlayerController::MoveRight(float Value)
{
	if (CurrentWidget)
	{
		if (CurrentWidget->myOwner)
			Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->MoveRight(Value);
	}
}

bool AComponentRPG_PlayerController::CallServerSetMoveToLocationCommand_Validate(APawn* InPawn, FVector move_location)
{
	return true;
}

void AComponentRPG_PlayerController::CallServerSetMoveToLocationCommand_Implementation(APawn* InPawn, FVector move_location)
{
	// hit ground
	Cast<AComponentRPGCharacter>(InPawn)->SetMoveToLocation(move_location);
	Cast<AComponentRPGCharacter>(InPawn)->CharStatComponent->CurrentStats.BASE_ACTIONBAR = 0.0f;
	Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CharacterExecuteAction(Cast<AComponentRPGCharacter>(InPawn));

	Cast<AComponentRPGCharacter>(InPawn)->SkillComponentDeck->GetMoveCommandComponent()->SetIsSelected(false);
}

void AComponentRPG_PlayerController::MouseClick()
{
	if (!CurrentWidget)
		return;

	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, false, Hit);

	///// look for character
	Hit.ImpactPoint.Y = -13.0f;	
	if (Hit.bBlockingHit)
	{
		AActor* trace = Hit.GetActor();
		if (trace)
		{
			if (Cast<AComponentRPGCharacter>(CurrentWidget->myOwner) != nullptr)
			{
				/////// MoveCommand logic
				//if (Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetMoveCommandComponent()->GetIsSelectingTarget())
				//{
				//	// hit ground
				//	TSubclassOf<UComponentRPG_UserWidget> TranslucentWidgetClass = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->TranslucentHUD_UI;
				//	if (TranslucentWidgetClass != nullptr)
				//	{
				//		UComponentRPG_UserWidget* tmpTargetUI = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), TranslucentWidgetClass);
				//		if (tmpTargetUI != nullptr)
				//		{
				//			tmpTargetUI->SetController(this);
				//			Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetMoveCommandComponent()->AddTargetLocation(Hit.ImpactPoint, tmpTargetUI);

				//			return;
				//		}
				//	}
				//}

				///// New MoveCommand logic
				if (Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetMoveCommandComponent()->GetIsSelected())
				{
					CallServerSetMoveToLocationCommand(Cast<AComponentRPGCharacter>(CurrentWidget->myOwner), Hit.ImpactPoint);
				}

				if (!Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetTargetingComponent()->GetIsSelectingTarget())
				{
					// look for projectile
					if (trace->IsA(AComponentRPG_BaseProjectile::StaticClass()))
					{
						FHitResult Hit2;
						GetHitResultUnderCursor(ECC_Pawn, false, Hit2);

						if (Hit2.bBlockingHit)
						{
							AActor* trace2 = Hit2.GetActor();
							if (trace2->IsA(AComponentRPGCharacter::StaticClass()))
							{
								CallServerPossess(Cast<AComponentRPGCharacter>(trace2));
								return;
							}
						}

						InspectedProjectile = Cast<AComponentRPG_BaseProjectile>(trace);
						return;
					}

					// Trace to possess new pawn
					if (trace->IsA(AComponentRPGCharacter::StaticClass()))
					{
						CallServerPossess(Cast<AComponentRPGCharacter>(trace));
						return;
					}

					//DrawDebugLine(GetWorld(), GetViewTarget()->GetActorLocation(), Hit.ImpactPoint, FColor(0, 255, 0, 150), false, 10.0f, 0, 5.0f);
				}
				else
				{
					// Targeting Component logic
					if (Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetTargetingComponent()->GetCurrentTargetType() == TargetType::TargetType_Character)
					{
						if (trace->IsA(AComponentRPGCharacter::StaticClass()))
						{
							// hit character
							TSubclassOf<UComponentRPG_UserWidget> TranslucentWidgetClass = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->TranslucentHUD_UI;
							if (TranslucentWidgetClass != nullptr)
							{
								UComponentRPG_UserWidget* tmpTargetUI = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), TranslucentWidgetClass);
								if (tmpTargetUI != nullptr)
								{
									tmpTargetUI->SetController(this);
									tmpTargetUI->SetWidgetOwner(trace);
									Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetTargetingComponent()->AddTargetCharacter(Cast<AComponentRPGCharacter>(trace), tmpTargetUI);
								}
							}
						}
					}
					else if (Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetTargetingComponent()->GetCurrentTargetType() == TargetType::TargetType_Ground)
					{
						// hit ground
						TSubclassOf<UComponentRPG_UserWidget> TranslucentWidgetClass = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->TranslucentHUD_UI;
						if (TranslucentWidgetClass != nullptr)
						{
							UComponentRPG_UserWidget* tmpTargetUI = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), TranslucentWidgetClass);
							if (tmpTargetUI != nullptr)
							{
								tmpTargetUI->SetController(this);
								Cast<AComponentRPGCharacter>(CurrentWidget->myOwner)->SkillComponentDeck->GetTargetingComponent()->AddTargetLocation(Hit.ImpactPoint, tmpTargetUI);
							}
						}
					}
					else
					{
						// TargetType::TargetType_None - do nothing
					}
				}
			}
			else
			{
				// look for projectile
				if (trace->IsA(AComponentRPG_BaseProjectile::StaticClass()))
				{
					FHitResult Hit2;
					GetHitResultUnderCursor(ECC_Pawn, false, Hit2);

					AActor* trace2 = Hit2.GetActor();
					if (trace2->IsA(AComponentRPGCharacter::StaticClass()))
					{
						CallServerPossess(Cast<AComponentRPGCharacter>(trace2));
						return;
					}

					InspectedProjectile = Cast<AComponentRPG_BaseProjectile>(trace);
					return;
				}

				// Trace to possess new pawn
				if (trace->IsA(AComponentRPGCharacter::StaticClass()))
				{
					CallServerPossess(Cast<AComponentRPGCharacter>(trace));
					return;
				}

				//// Trace to possess new pawn
				//if (trace->IsA(AComponentRPGCharacter::StaticClass()))
				//{
				//	CallServerPossess(Cast<AComponentRPGCharacter>(trace));
				//	return;
				//}

				//// look for projectile
				//if (trace->IsA(AComponentRPG_BaseProjectile::StaticClass()))
				//{
				//	InspectedProjectile = Cast<AComponentRPG_BaseProjectile>(trace);
				//	return;
				//}
			}
		}
	}
}

bool AComponentRPG_PlayerController::CallServerPossess_Validate(APawn* InPawn)
{
	return true;
}

void AComponentRPG_PlayerController::CallServerPossess_Implementation(APawn* InPawn)
{
	/*if (Cast<AComponentRPGCharacter>(InPawn)->TeamIndex == ControllerIndex)
		Possess(InPawn);*/
	
	// just change UI target without possess the pawn
	ClientChangeWidgetOwner(InPawn);
	
}

void AComponentRPG_PlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
}

void AComponentRPG_PlayerController::ClientChangeWidgetOwner_Implementation(APawn* InPawn)
{
	// update controller HUD to new character
	ChangeWidgetOwner(InPawn);
	InspectedProjectile = nullptr;
}

void AComponentRPG_PlayerController::ChangeWidgetOwner(APawn* InPawn)
{
	// update controller HUD to new character
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->SetWidgetOwner(InPawn);
	}
}

void AComponentRPG_PlayerController::SetControllerIndex_Implementation(uint32 index)
{
	ControllerIndex = index;
}

void AComponentRPG_PlayerController::AddWidgetOnController_Implementation(TSubclassOf<UComponentRPG_UserWidget> widget_template, UObject* widget_owner)
{
	///// setup HUD
	UComponentRPG_UserWidget* tmpWidget;
	if (widget_template != nullptr)
	{
		tmpWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), widget_template);
		if (tmpWidget != nullptr)
		{
			tmpWidget->SetWidgetOwner(widget_owner);
			tmpWidget->AddToViewport();
		}
	}
}


///// SkillComponent RPC
bool AComponentRPG_PlayerController::SkillComponent_SetIsSelected_Validate(UComponentRPG_SkillComponent* skill_component, bool val)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_SetIsSelected_Implementation(UComponentRPG_SkillComponent* skill_component, bool val)
{
	skill_component->SetIsSelected(val);
}

//
bool AComponentRPG_PlayerController::SkillComponent_SetTargetingType_Validate(UComponentRPG_SkillComponent* skill_component, TargetingType val)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_SetTargetingType_Implementation(UComponentRPG_SkillComponent* skill_component, TargetingType val)
{
	skill_component->SetTargetingType(val);
}

//
bool AComponentRPG_PlayerController::SkillComponent_SetTargetSideType_Validate(UComponentRPG_SkillComponent* skill_component, TargetSideType val)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_SetTargetSideType_Implementation(UComponentRPG_SkillComponent* skill_component, TargetSideType val)
{
	skill_component->SetTargetSideType(val);
}

//
bool AComponentRPG_PlayerController::SkillComponent_SetTargetType_Validate(UComponentRPG_SkillComponent* skill_component, TargetType val)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_SetTargetType_Implementation(UComponentRPG_SkillComponent* skill_component, TargetType val)
{
	skill_component->SetTargetType(val);
}

//
bool AComponentRPG_PlayerController::SkillComponent_StartSelectingTarget_Validate(UComponentRPG_SkillComponent* skill_component)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_StartSelectingTarget_Implementation(UComponentRPG_SkillComponent* skill_component)
{
	skill_component->StartSelectingTarget();
}

//
bool AComponentRPG_PlayerController::SkillComponent_StopSelectingTarget_Validate(UComponentRPG_SkillComponent* skill_component)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_StopSelectingTarget_Implementation(UComponentRPG_SkillComponent* skill_component)
{
	skill_component->StopSelectingTarget();
}

//
bool AComponentRPG_PlayerController::SkillComponent_ClearAllTarget_Validate(UComponentRPG_SkillComponent* skill_component)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_ClearAllTarget_Implementation(UComponentRPG_SkillComponent* skill_component)
{
	skill_component->ClearAllTarget();
}

//
bool AComponentRPG_PlayerController::SkillComponent_AddTargetCharacterToArray_Validate(UComponentRPG_SkillComponent* skill_component, ACharacter* targetChar)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_AddTargetCharacterToArray_Implementation(UComponentRPG_SkillComponent* skill_component, ACharacter* targetChar)
{
	skill_component->AddTargetCharacterToArray(targetChar);
}

//
bool AComponentRPG_PlayerController::SkillComponent_AddTargetLocationToArray_Validate(UComponentRPG_SkillComponent* skill_component, FVector targetLoc)
{
	return true;
}
void AComponentRPG_PlayerController::SkillComponent_AddTargetLocationToArray_Implementation(UComponentRPG_SkillComponent* skill_component, FVector targetLoc)
{
	skill_component->AddTargetLocationToArray(targetLoc);
}

void AComponentRPG_PlayerController::ClientCreateHUD_StartCombat_Implementation()
{
	AComponentRPG_GameState* myGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	// hide login HUD
	if (NonGameplay_CurrentWidget != nullptr)
	{
		NonGameplay_CurrentWidget->RemoveFromViewport();
		NonGameplay_CurrentWidget = nullptr;
	}

	///// setup Controller HUD
	TSubclassOf<UComponentRPG_UserWidget> StartingWidgetClass = myGameState->PlayerController_UI;
	if (StartingWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), StartingWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->SetController(this);
			CurrentWidget->SetWidgetOwner(GetCharacter());
			CurrentWidget->AddToViewport();
		}
	}
}

///// HTTP land
bool AComponentRPG_PlayerController::Server_Http_LoadProfile_Validate(const FString& username)
{
	return true;
}

void AComponentRPG_PlayerController::Server_Http_LoadProfile_Implementation(const FString& username)
{
	HttpRequest_LoadPlayerProfile(username);
}

void AComponentRPG_PlayerController::HttpRequest_LoadPlayerProfile(FString username)
{
	AComponentRPG_GameState* gamestate = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	gamestate->HttpRequest_LoadPlayerProfile(username, ControllerIndex);
}

void AComponentRPG_PlayerController::HttpResponse_LoadPlayerProfile(UComponentRPG_SaveProfile* save_profile)
{
	mySaveProfile = save_profile;

	// validate save file here
	if (mySaveProfile->CharProfileArray.Num() >= 3)
	{
		// load save correctly, start the game
		AComponentRPG_GameState* myGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
		myGameState->GameStateInit(ControllerIndex, mySaveProfile);

		ClientCreateHUD_StartCombat();
	}
}
///// end of HTTP land

///// Menu widget UI (non-gameplay widget)
void AComponentRPG_PlayerController::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (NonGameplay_CurrentWidget != nullptr)
    {
        NonGameplay_CurrentWidget->RemoveFromViewport();
        NonGameplay_CurrentWidget = nullptr;
    }

    if (NewWidgetClass != nullptr)
    {
		NonGameplay_CurrentWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), NewWidgetClass);
        if (NonGameplay_CurrentWidget != nullptr)
        {
			NonGameplay_CurrentWidget->SetController(this);
            NonGameplay_CurrentWidget->AddToViewport();
        }
    }
}