// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_TranslucentHUD.h"
#include "RPG_SkillComponent_Targeting.h"
#include "ComponentRPG_SkillComponentDeck.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_PlayerController.h"


URPG_SkillComponent_Targeting::URPG_SkillComponent_Targeting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

void URPG_SkillComponent_Targeting::Init(void* obj)
{
	Super::Init(obj);

	m_SkillComponentType = SkillComponentType::SkillComponentType_Targeting;
	SetIsSelected(true);
	canPickTarget = true;

	CastRateModifier = 1.0f;

	IndicatorColor = FLinearColor(0.0f, 0.5f, 0.0f, 0.5f);
}

void URPG_SkillComponent_Targeting::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URPG_SkillComponent_Targeting, targeting_instruction);
	DOREPLIFETIME(URPG_SkillComponent_Targeting, isSelectingTarget);
	DOREPLIFETIME(URPG_SkillComponent_Targeting, canPickTarget);
	DOREPLIFETIME(URPG_SkillComponent_Targeting, TargetCharacterArray);
	DOREPLIFETIME(URPG_SkillComponent_Targeting, TargetLocationArray);
}

void URPG_SkillComponent_Targeting::ClearAllTarget()
{
	TargetCharacterArray.Empty();
	int32 count = TargetCharacterArray_UI.Num();
	for (int32 i = 0; i < count; ++i)
	{
		TargetCharacterArray_UI[0]->RemoveFromParent();
		TargetCharacterArray_UI[0]->RemoveFromViewport();
		TargetCharacterArray_UI.RemoveAt(0);
	}
	TargetCharacterArray_UI.Empty();

	TargetLocationArray.Empty();
	count = TargetLocationArray_UI.Num();
	for (int32 i = 0; i < count; ++i)
	{
		TargetLocationArray_UI[0]->RemoveFromParent();
		TargetLocationArray_UI[0]->RemoveFromViewport();
		TargetLocationArray_UI.RemoveAt(0);
	}
	TargetLocationArray_UI.Empty();
}

void URPG_SkillComponent_Targeting::ServerClearAllTarget()
{
	if (ComponentOwner == nullptr)
		return;

	if (Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController() == nullptr)
		return;

	Cast<AComponentRPG_PlayerController>(Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController())->SkillComponent_ClearAllTarget(this);
	SetClientClearTargetingComponent();
}

void URPG_SkillComponent_Targeting::SetClientClearTargetingComponent()
{
	Cast<AComponentRPGCharacter>(ComponentOwner)->SetClientClearTargetingComponent(true);
}

void URPG_SkillComponent_Targeting::EnableCanPickTarget()
{
	canPickTarget = true;
}

bool URPG_SkillComponent_Targeting::GetIsSelectingTarget()
{
	return isSelectingTarget;
}

void URPG_SkillComponent_Targeting::StartSelectingTarget()
{
	isSelectingTarget = true;
	canPickTarget = true;
	ClearAllTarget();
	ServerClearAllTarget();
}

void URPG_SkillComponent_Targeting::ShowCurrentTargetUI()
{

}

void URPG_SkillComponent_Targeting::StopSelectingTarget()
{
	isSelectingTarget = false;
}

TargetType URPG_SkillComponent_Targeting::GetCurrentTargetType()
{
	if (isSelectingTarget)
	{
		return targeting_instruction.target_type;
	}
	else
	{ 
		return TargetType::TargetType_None;
	}
}

void URPG_SkillComponent_Targeting::AddTargetCharacter(ACharacter* targetChar, UComponentRPG_UserWidget* translucentHUD)
{
	if (isSelectingTarget && targetChar != nullptr && translucentHUD != nullptr)
	{
		UComponentRPG_TranslucentHUD* tmpTranslucantHUD = Cast<UComponentRPG_TranslucentHUD>(translucentHUD);
		tmpTranslucantHUD->AddToViewport();
		//tmpTranslucantHUD->SetSquareIndicator();
		tmpTranslucantHUD->SetIndicatorSize(1, 1);
		tmpTranslucantHUD->SetIndicatorColor(IndicatorColor);

		canPickTarget = false;
		//AddTargetCharacterToArray(targetChar);
		ServerAddTargetCharacterToArray(targetChar);

		TargetCharacterArray_UI.Add(translucentHUD);

		CheckStopTargeting();
	}
}

void URPG_SkillComponent_Targeting::AddTargetCharacterToArray(ACharacter* targetChar)
{
	TargetCharacterArray.Add(targetChar);
}
void URPG_SkillComponent_Targeting::ServerAddTargetCharacterToArray(ACharacter* targetChar)
{
	if (ComponentOwner == nullptr)
		return;

	if (Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController() == nullptr)
		return;

	Cast<AComponentRPG_PlayerController>(Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController())->SkillComponent_AddTargetCharacterToArray(this, targetChar);
}


void URPG_SkillComponent_Targeting::AddTargetLocation(FVector targetLoc, UComponentRPG_UserWidget* translucentHUD)
{
	if (isSelectingTarget)
	{
		UComponentRPG_TranslucentHUD* tmpTranslucantHUD = Cast<UComponentRPG_TranslucentHUD>(translucentHUD);
		tmpTranslucantHUD->AddToViewport();
		tmpTranslucantHUD->SetCircleIndicator();
		tmpTranslucantHUD->SetIndicatorSize(GetAreaSize(), GetAreaSize());
		tmpTranslucantHUD->SetIndicatorColor(IndicatorColor);
		tmpTranslucantHUD->SetHUDPos(targetLoc);


		//AddTargetLocationToArray(targetLoc);
		ServerAddTargetLocationToArray(targetLoc);

		TargetLocationArray_UI.Add(translucentHUD);

		CheckStopTargeting();
	}
}

void URPG_SkillComponent_Targeting::AddTargetLocationToArray(FVector targetLoc)
{
	TargetLocationArray.Add(targetLoc);
}
void URPG_SkillComponent_Targeting::ServerAddTargetLocationToArray(FVector targetLoc)
{
	if (ComponentOwner == nullptr)
		return;

	if (Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController() == nullptr)
		return;

	Cast<AComponentRPG_PlayerController>(Cast<AComponentRPGCharacter>(ComponentOwner)->GetWorld()->GetFirstPlayerController())->SkillComponent_AddTargetLocationToArray(this, targetLoc);
}

void URPG_SkillComponent_Targeting::CheckStopTargeting()
{
	if (TargetCharacterArray.Num() >= GetNumTarget() || TargetLocationArray.Num() >= GetNumTarget())
	{
		StopSelectingTarget();
	}
}

void URPG_SkillComponent_Targeting::SetTargetingType(TargetingType val)
{
	if (val == TargetingType::TargetingType_Missile)
	{
		targeting_instruction.area_size = 0.2f;
	}
	else if (val == TargetingType::TargetingType_AOE)
	{
		targeting_instruction.area_size = 1.0f;
	}
	else if (val == TargetingType::TargetingType_Melee)
	{
		targeting_instruction.area_size = 0.2f;
	}

	targeting_instruction.targeting_type = val;
	ClearAllTarget();
	ServerClearAllTarget();
}

void URPG_SkillComponent_Targeting::SetTargetSideType(TargetSideType val)
{
	targeting_instruction.target_side = val;
	ClearAllTarget();
	ServerClearAllTarget();
}

void URPG_SkillComponent_Targeting::SetTargetType(TargetType val)
{
	targeting_instruction.target_type = val;
	ClearAllTarget();
	ServerClearAllTarget();
}

TargetingType URPG_SkillComponent_Targeting::GetTargetingType()
{
	return targeting_instruction.targeting_type;
}

TargetSideType URPG_SkillComponent_Targeting::GetTargetSideType()
{
	return targeting_instruction.target_side;
}

TargetType URPG_SkillComponent_Targeting::GetTargetType()
{
	return targeting_instruction.target_type;
}

void URPG_SkillComponent_Targeting::ResetDefaultValue()
{
	targeting_instruction.num_of_target = 1;
	targeting_instruction.area_size = 0.2f;

	targeting_instruction.targeting_type = TargetingType::TargetingType_Missile;
	targeting_instruction.target_side = TargetSideType::TargetSideType_Both;
	targeting_instruction.target_type = TargetType::TargetType_Character;
}

void URPG_SkillComponent_Targeting::RandomizeComponent()
{

}

int32 URPG_SkillComponent_Targeting::GetNumTarget()
{
	return targeting_instruction.num_of_target + GetSumAdditionalNumTarget();
}

float URPG_SkillComponent_Targeting::GetAreaSize()
{
	return targeting_instruction.area_size + GetSumAdditionalAreaSize();
}


int32 URPG_SkillComponent_Targeting::GetSumAdditionalNumTarget()
{
	if (Cast<AComponentRPGCharacter>(ComponentOwner) != nullptr)
	{
		if (Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck != nullptr)
			return Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetSumAdditionalNumTarget();
	}
	else if (Cast<AComponentRPG_BaseProjectile>(ComponentOwner) != nullptr)
	{
		if (Cast<AComponentRPG_BaseProjectile>(ComponentOwner)->SkillComponentDeck != nullptr)
			return Cast<AComponentRPG_BaseProjectile>(ComponentOwner)->SkillComponentDeck->GetSumAdditionalNumTarget();
	}

	return 0;
}

float URPG_SkillComponent_Targeting::GetSumAdditionalAreaSize()
{
	if (Cast<AComponentRPGCharacter>(ComponentOwner) != nullptr)
		return Cast<AComponentRPGCharacter>(ComponentOwner)->SkillComponentDeck->GetSumAdditionalAreaSize();
	else if (Cast<AComponentRPG_BaseProjectile>(ComponentOwner) != nullptr)
		return Cast<AComponentRPG_BaseProjectile>(ComponentOwner)->SkillComponentDeck->GetSumAdditionalAreaSize();

	return 0.0f;
}

FString URPG_SkillComponent_Targeting::GetSkillComponentDescription()
{
	FString result = "";
	result += "CharArr=" + FString::FromInt(TargetCharacterArray.Num());
	for (int i = 0; i < TargetCharacterArray.Num(); ++i)
	{
		result += "\n";
		result += TargetCharacterArray[i]->GetName();
	}
	result += "\n";
	result += ",LocArr=" + FString::FromInt(TargetLocationArray.Num());
	for (int i = 0; i < TargetLocationArray.Num(); ++i)
	{
		result += "\n";
		result += "(" + FString::FromInt(TargetLocationArray[i].X);
		result += "," + FString::FromInt(TargetLocationArray[i].Y);
		result += "," + FString::FromInt(TargetLocationArray[i].Z);
		result += ")";
	}
	return result;
}