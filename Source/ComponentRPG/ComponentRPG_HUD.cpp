// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_HUD.h"

#include "Engine/Canvas.h"
#include "Engine/Font.h"

#include "ComponentRPG_PlayerController.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_GameState.h"


AComponentRPG_HUD::AComponentRPG_HUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	//ATennisPlayerHUD::ATennisPlayerHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontOb.Object;
}

void AComponentRPG_HUD::DrawHUD()
{
	Super::DrawHUD();
	FString ControllerStatus;

	if (Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld())) != nullptr)
	{
		if (Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CurrentCharacterTurn != nullptr)
		{
			if (Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CurrentCharacterTurn->bWaitingForAction)
			{
				ControllerStatus += "TurnTimeRemaining=";
				ControllerStatus += FString::SanitizeFloat(10.0f - GetWorld()->TimeSince( Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CurrentCharacterStartTurnTime ));

				//ControllerStatus += "\nTurnName=";
				//ControllerStatus += Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CurrentCharacterTurn->GetName();
			}
		}
	}

	AComponentRPG_PlayerController* tmpController = Cast<AComponentRPG_PlayerController>(GetOwningPlayerController());

	ControllerStatus += "\nControllerIndex=";
	ControllerStatus += FString::FromInt( tmpController->ControllerIndex );

	ControllerStatus += "\nSaveProfile=";
	if (tmpController->mySaveProfile)
	{
		for (int i = 0; i < tmpController->mySaveProfile->CharProfileArray.Num(); ++i)
		{
			ControllerStatus += "\n" + tmpController->mySaveProfile->CharProfileArray[i].CharName;
		}
	}
	else
	{
		ControllerStatus += "NULL";
	}

	if (tmpController->CurrentWidget == nullptr)
		return;

	AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>( Cast<AComponentRPG_PlayerController>(GetOwningPlayerController())->CurrentWidget->myOwner );
	if (Cast<AComponentRPG_PlayerController>(GetOwningPlayerController())->InspectedProjectile != nullptr)
		ControllerStatus += GetProjectileInfo(Cast<AComponentRPG_PlayerController>(GetOwningPlayerController())->InspectedProjectile);
	else
		ControllerStatus += GetCharInfo(tmpChar);
	
	
	
	DrawText(ControllerStatus, FColor::White, Canvas->SizeX-500, 20, HUDFont);


	//FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	//Super::DrawHUD();
	
	//if (UGameplayStatics::GetPlayerPawn(this, 0))
	//{
	//	AComponentRPGCharacter* myCharacter = Cast<AComponentRPGCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//	UComponentRPG_CharStatComponent* tmp = Cast<UComponentRPG_CharStatComponent>(myCharacter->GetComponentByClass(UComponentRPG_CharStatComponent::StaticClass()));


	//	//ATennisTopDownPlayerController* myController = Cast<ATennisTopDownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//
	//	FString ControllerStatus;
	//	//ControllerStatus += FString::Printf(TEXT("isSwingButtonDown = %s\n"), myCharacter->UI_Response);
	//	ControllerStatus += myCharacter->UI_Response;

	//	ControllerStatus += "\nHP=";
	//	ControllerStatus += FString::SanitizeFloat(tmp->CurrentStats.BASE_HP);

	//	FVector boardLoc = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->GameManager->BoardPosition;
	//	ControllerStatus += boardLoc.ToString();

	//	DrawText(ControllerStatus, FColor::White, 20, 20, HUDFont);
	//}
}

FString AComponentRPG_HUD::GetProjectileInfo(AComponentRPG_BaseProjectile* tmpProj)
{
	FString ControllerStatus = "";
	if (tmpProj != nullptr)
	{
		TArray<UComponentRPG_SkillComponent*> testTypeArr = tmpProj->SkillComponentDeck->SkillComponentArray;

		ControllerStatus += "\nTimeDilation=";
		ControllerStatus += FString::SanitizeFloat(tmpProj->myCustomTimeDilation);

		ControllerStatus += "\nLifeSpan=";
		ControllerStatus += FString::SanitizeFloat(tmpProj->GetLifeSpan());

		ControllerStatus += "\nDestPos=";
		ControllerStatus += "(" + FString::FromInt(tmpProj->DestPosition.X);
		ControllerStatus += "," + FString::FromInt(tmpProj->DestPosition.Y);
		ControllerStatus += "," + FString::FromInt(tmpProj->DestPosition.Z);
		ControllerStatus += ")";

		ControllerStatus += "\nCharLoc=";
		ControllerStatus += "(" + FString::FromInt(tmpProj->GetActorLocation().X);
		ControllerStatus += "," + FString::FromInt(tmpProj->GetActorLocation().Y);
		ControllerStatus += "," + FString::FromInt(tmpProj->GetActorLocation().Z);
		ControllerStatus += ")";

		for (int i = 0; i < testTypeArr.Num(); ++i)
		{
			ControllerStatus += "\n";
			ControllerStatus += testTypeArr[i]->GetSkillComponentDescription();
			ControllerStatus += "; ";
		}

		ControllerStatus += "\nCurrentStat_DMG=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_DAMAGE);
		ControllerStatus += "\nCurrentStat_EFF=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_EFFECTIVENESS);
		ControllerStatus += "\nCurrentStat_DEF=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_DEFENSE);
		ControllerStatus += "\nCurrentStat_RES=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_RESISTANCE);
		ControllerStatus += "\nCurrentStat_SPD=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_SPEED);
		ControllerStatus += "\nCurrentStat_RCV=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_RECOVERY);
		ControllerStatus += "\nCurrentStat_BAR=" + FString::SanitizeFloat(tmpProj->CharStatComponent->CurrentStatPoints.STATPOINT_MAXBAR);
	}

	return ControllerStatus;
}

FString AComponentRPG_HUD::GetCharInfo(AComponentRPGCharacter* tmpChar)
{
	FString ControllerStatus = "";
	if (tmpChar != nullptr)
	{
		UComponentRPG_TestRep* testRep = tmpChar->testRep;
		TArray<USkillComponentArrayByType*> testTypeArr = tmpChar->SkillComponentDeck->SkillComponentArrayByType;
		TArray<UBuffArrayByType*> testBuffTypeArr = tmpChar->BuffDeck->BuffArrayByType;
		if (testRep != nullptr)
		{
			ControllerStatus += "\nCharTeam=";
			ControllerStatus += FString::FromInt(tmpChar->TeamIndex);

			ControllerStatus += "\nDeckOwner=";
			ControllerStatus += tmpChar->SkillComponentDeck->DeckOwner->GetName();

			ControllerStatus += "\nCharController=";
			if (tmpChar->GetController() != nullptr)
				ControllerStatus += tmpChar->GetController()->GetName();
			else
				ControllerStatus += "NULL";

			ControllerStatus += "\nbWaitingForAction=" + FString::FromInt(tmpChar->bWaitingForAction);

			//ControllerStatus += "\nTimeDilation=";
			//ControllerStatus += FString::SanitizeFloat(tmpChar->myCustomTimeDilation);

			ControllerStatus += "\nCharLoc=";
			ControllerStatus += "(" + FString::FromInt(tmpChar->GetActorLocation().X);
			ControllerStatus += "," + FString::FromInt(tmpChar->GetActorLocation().Y);
			ControllerStatus += "," + FString::FromInt(tmpChar->GetActorLocation().Z);
			ControllerStatus += ")";

			ControllerStatus += "\nCastRateModifier=" + FString::SanitizeFloat(tmpChar->SkillComponentDeck->GetTotalCastRateModifier());
			ControllerStatus += "\nIsMeleeAttacking=" + FString::FromInt(tmpChar->IsMeleeAttacking);

			for (int i = 0; i < testTypeArr.Num(); ++i)
			{
				for (int j = 0; j < testTypeArr[i]->SkillComponentArray.Num(); ++j)
				{
					ControllerStatus += "\n";
					ControllerStatus += testTypeArr[i]->SkillComponentArray[j]->GetSkillComponentDescription();
					ControllerStatus += "; ";
				}
			}

			ControllerStatus += "\n===Buff===";
			/*for (int i = 0; i < testBuffTypeArr.Num(); ++i)
			{
				for (int j = 0; j < testBuffTypeArr[i]->BuffArray.Num(); ++j)
				{
					if (testBuffTypeArr[i]->BuffArray[j] != nullptr)
					{
						ControllerStatus += "\n";
						ControllerStatus += testBuffTypeArr[i]->BuffArray[j]->GetBuffDescription();
						ControllerStatus += "; ";
					}
				}
			}*/

			TArray<AComponentRPG_BuffActor*> testBuffTypeArr = tmpChar->BuffDeck->BuffArray;
			for (int i = 0; i < testBuffTypeArr.Num(); ++i)
			{
				if (testBuffTypeArr[i] != nullptr)
				{
					ControllerStatus += "\n";
					ControllerStatus += testBuffTypeArr[i]->GetBuffDescription();
					ControllerStatus += "; ";
				}
			}

			




			//ControllerStatus += "\nTestRep=";
			//if (testRep->testComp)
			//	ControllerStatus += testRep->testComp->GetBuffDescription();

			//for (int i = 0; i < testRep->testCompArray.Num(); ++i)
			//{
			//ControllerStatus += "\n";
			//ControllerStatus += testRep->testCompArray[i]->GetSkillComponentDescription();
			//}
		}
	}

	return ControllerStatus;
}