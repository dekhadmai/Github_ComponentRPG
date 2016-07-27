// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "RPG_SkillComponent_Targeting.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_AIController.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_BaseProjectile.h"
#include "ComponentRPG_SkillComponentDeck.h"


UComponentRPG_SkillComponentDeck::UComponentRPG_SkillComponentDeck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Init();
}

bool UComponentRPG_SkillComponentDeck::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = false;

	for (UComponentRPG_SkillComponent * myObject : SkillComponentArray)
	{
		if (myObject != nullptr)
		{
			WroteSomething |= Channel->ReplicateSubobject(myObject, *Bunch, *RepFlags);
		}
	}

	for (USkillComponentArrayByType * myObjectByType : SkillComponentArrayByType)
	{
		if (myObjectByType != nullptr)
		{
			WroteSomething |= Channel->ReplicateSubobject(myObjectByType, *Bunch, *RepFlags);
			WroteSomething |= myObjectByType->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}

	return WroteSomething;
}

void UComponentRPG_SkillComponentDeck::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UComponentRPG_SkillComponentDeck, SkillComponentArray);
	DOREPLIFETIME(UComponentRPG_SkillComponentDeck, SkillComponentArrayByType);
	DOREPLIFETIME(UComponentRPG_SkillComponentDeck, DeckOwner);
}

void UComponentRPG_SkillComponentDeck::SetDeckOwner(UObject* owner)
{
	DeckOwner = owner;
}

void UComponentRPG_SkillComponentDeck::Init()
{
	for (int i = 0; i < (uint8)SkillComponentType::SkillComponentType_Max; ++i)
	{
		SkillComponentArrayByType.Add(NewObject<USkillComponentArrayByType>(this));
	}
}

void UComponentRPG_SkillComponentDeck::AddSkillComponentToDeck(UComponentRPG_SkillComponent* comp)
{	
	comp->SetOwner(DeckOwner);
	SkillComponentArray.Add(comp);
	SkillComponentArrayByType[(uint8)comp->GetSkillComponentType()]->SkillComponentArray.Add(comp);
}

UComponentRPG_SkillComponent* UComponentRPG_SkillComponentDeck::GetTargetingComponent()
{
	if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_Targeting]->SkillComponentArray[0] != nullptr)
		return SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_Targeting]->SkillComponentArray[0];
	else
		return nullptr;
}

UComponentRPG_SkillComponent* UComponentRPG_SkillComponentDeck::GetMoveCommandComponent()
{
	if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_MoveCommand]->SkillComponentArray[0] != nullptr)
		return SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_MoveCommand]->SkillComponentArray[0];
	else
		return nullptr;
}

///// Helper function
int32 UComponentRPG_SkillComponentDeck::GetSumAdditionalNumTarget()
{
	int32 result = 0;
	for (int32 i = 0; i < SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray.Num(); ++i)
	{
		if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray[i]->GetIsSelected())
			result += Cast<URPG_SkillComponent_PlusNumTarget>(SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray[i])->AdditionalNumTarget;
	}

	return result;
}

float UComponentRPG_SkillComponentDeck::GetSumAdditionalAreaSize()
{
	float result = 0;
	for (int32 i = 0; i < SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray.Num(); ++i)
	{
		if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray[i]->GetIsSelected())
			result += Cast<URPG_SkillComponent_PlusAreaSize>(SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray[i])->AdditionalAreaSize;
	}

	return result;
}

float UComponentRPG_SkillComponentDeck::GetTotalCastRateModifier()
{
	float result = 1;
	for (int32 i = 0; i < SkillComponentArray.Num(); ++i)
	{
		if (SkillComponentArray[i]->GetIsSelected())
			result *= SkillComponentArray[i]->GetSkillComponentCastRateModifier();
	}

	return result;
}

void UComponentRPG_SkillComponentDeck::ExecuteAction()
{
	///// grab all components and do something
	AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(DeckOwner);
	//if (Cast<URPG_SkillComponent_Targeting>(GetMoveCommandComponent())->TargetLocationArray.Num() > 0)
	//{
	//	// found move location, tell the character to move
	//	tmpChar->SetMoveToLocation(Cast<URPG_SkillComponent_Targeting>(GetMoveCommandComponent())->TargetLocationArray[0]);
	//	Cast<AComponentRPGCharacter>(DeckOwner)->SetClientClearMovementComponent(true);
	//	Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR = 0.0f;
	//}
	//else
	//{
		//bool CheckMoveComp = Cast<URPG_SkillComponent_Targeting>(GetMoveCommandComponent())->TargetLocationArray.Num() > 0;
		bool CheckMoveComp = GetMoveCommandComponent()->GetIsSelected();
		bool CheckTargetingComp = (Cast<URPG_SkillComponent_Targeting>(GetTargetingComponent())->TargetLocationArray.Num() +
			Cast<URPG_SkillComponent_Targeting>(GetTargetingComponent())->TargetCharacterArray.Num()) > 0;

		if (CheckMoveComp || CheckTargetingComp)
		{
			///// cast skill
			tmpChar->StartCastingSkill();
		}
		else
		{
			///// Skip turn
			Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR = Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR * 0.5f;
			Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_CASTBAR = 0.0f;
		}
	//}

	///// clean up
	Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(Cast<AComponentRPGCharacter>(DeckOwner)->GetWorld()))->CharacterExecuteAction(Cast<AComponentRPGCharacter>(DeckOwner));
}

void UComponentRPG_SkillComponentDeck::FinishCastingSkill()
{
	AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(DeckOwner);
	AComponentRPG_BaseProjectile* tmpProjectile;
	URPG_SkillComponent_Targeting* tmpTargetingComponent = Cast<URPG_SkillComponent_Targeting>(GetTargetingComponent());
	if (tmpTargetingComponent->GetTargetingType() == TargetingType::TargetingType_Missile)
	{
		///// Missile Attack
		if (tmpTargetingComponent->TargetLocationArray.Num() > 0)
		{
			// target ground
			for (int i = 0; i < tmpTargetingComponent->TargetLocationArray.Num(); ++i)
			{
				FVector tmpProjDirection = tmpTargetingComponent->TargetLocationArray[i] - tmpChar->GetActorLocation();
				tmpProjDirection.Normalize();
				tmpProjectile = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(tmpChar->GetWorld()))->SpawnProjectileActor(tmpChar->GetActorLocation(), tmpProjDirection.Rotation(), tmpChar);

				if (tmpProjectile)
				{
					tmpProjectile->DestPosition = tmpTargetingComponent->TargetLocationArray[i];
					ProjectileCommonSetup(tmpProjectile, tmpChar);
				}
			}
		}
		else if (tmpTargetingComponent->TargetCharacterArray.Num() > 0)
		{
			// target character (homing)
			for (int i = 0; i < tmpTargetingComponent->TargetCharacterArray.Num(); ++i)
			{
				FVector tmpProjDirection = tmpTargetingComponent->TargetCharacterArray[i]->GetActorLocation() - tmpChar->GetActorLocation();
				tmpProjDirection.Normalize();
				tmpProjectile = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(tmpChar->GetWorld()))->SpawnProjectileActor(tmpChar->GetActorLocation(), tmpProjDirection.Rotation(), tmpChar);

				if (tmpProjectile)
				{
					tmpProjectile->DestActor = tmpTargetingComponent->TargetCharacterArray[i];
					tmpProjectile->SetInitialSpeed(tmpProjectile->myInitialSpeed * 0.5f);
					ProjectileCommonSetup(tmpProjectile, tmpChar);
				}
			}
		}
	}
	else if (tmpTargetingComponent->GetTargetingType() == TargetingType::TargetingType_AOE)
	{
		///// AOE Attack
		if (tmpTargetingComponent->TargetLocationArray.Num() > 0)
		{
			// target ground
			for (int i = 0; i < tmpTargetingComponent->TargetLocationArray.Num(); ++i)
			{
				FVector tmpProjDirection = tmpTargetingComponent->TargetLocationArray[i] - tmpChar->GetActorLocation();
				tmpProjDirection.Normalize();
				tmpProjectile = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(tmpChar->GetWorld()))->SpawnProjectileActor(tmpChar->GetActorLocation(), tmpProjDirection.Rotation(), tmpChar);

				if (tmpProjectile)
				{
					tmpProjectile->DestPosition = tmpTargetingComponent->TargetLocationArray[i];
					ProjectileCommonSetup(tmpProjectile, tmpChar);
					tmpProjectile->SetTargetingTypeAOE();
				}
			}
		}
		else if (tmpTargetingComponent->TargetCharacterArray.Num() > 0)
		{
			// target character (homing)
			///// AOE Shouldn't home?, Disable the button later
		}
	}
	else if (tmpTargetingComponent->GetTargetingType() == TargetingType::TargetingType_Melee)
	{
		///// Melee Attack
		if (tmpTargetingComponent->TargetLocationArray.Num() > 0)
		{
			// target ground
			tmpChar->SetMoveToLocation(tmpTargetingComponent->TargetLocationArray[0]);
			tmpChar->SetIsMeleeAttacking(true);
		}
		else if (tmpTargetingComponent->TargetCharacterArray.Num() > 0)
		{
			// target character
			tmpChar->SetMoveToActor(tmpTargetingComponent->TargetCharacterArray[0]);
			tmpChar->SetIsMeleeAttacking(true);
		}
	}

	///// clean up
	Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR = 0.0f;
	Cast<AComponentRPGCharacter>(DeckOwner)->CharStatComponent->CurrentStats.BASE_CASTBAR = 0.0f;
	Cast<AComponentRPGCharacter>(DeckOwner)->ConsecutiveCastInterruptionCount = 0;
	Cast<AComponentRPGCharacter>(DeckOwner)->SetClientClearTargetingComponent(true);
}

void UComponentRPG_SkillComponentDeck::ProjectileCommonSetup(AComponentRPG_BaseProjectile* tmp_proj, AComponentRPGCharacter* tmp_char)
{
	///// common projectile setup
	tmp_proj->TeamIndex = tmp_char->TeamIndex;
	tmp_proj->ProjOwner = tmp_char;
	tmp_proj->Instigator = tmp_char;
	tmp_proj->SetActorLifeSpan(10.0f);

	// add skill comp from deck to projectile here
	for (int i = 0; i < SkillComponentArray.Num(); ++i)
	{
		if (SkillComponentArray[i]->GetIsSelected())
		{
			tmp_proj->AddDuplicateSkillComponentToDeck(SkillComponentArray[i]);
		}
	}

	// add stat
	tmp_proj->CharStatComponent = DuplicateObject(tmp_char->CharStatComponent, tmp_char->GetOuter());
}