// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_BuffDeck.h"


UComponentRPG_BuffDeck::UComponentRPG_BuffDeck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Init();
}

bool UComponentRPG_BuffDeck::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = false;

	//for (AComponentRPG_BuffActor * myObject : BuffArray)
	//{
	//	if (myObject != nullptr)
	//	{
	//		WroteSomething |= Channel->ReplicateSubobject(myObject, *Bunch, *RepFlags);
	//	}
	//}

	for (UBuffArrayByType * myObjectByType : BuffArrayByType)
	{
		if (myObjectByType != nullptr)
		{
			WroteSomething |= Channel->ReplicateSubobject(myObjectByType, *Bunch, *RepFlags);
			//WroteSomething |= myObjectByType->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}

	return WroteSomething;
}

void UComponentRPG_BuffDeck::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UComponentRPG_BuffDeck, BuffArray);
	DOREPLIFETIME(UComponentRPG_BuffDeck, BuffArrayByType);
	DOREPLIFETIME(UComponentRPG_BuffDeck, DeckOwner);
}

void UComponentRPG_BuffDeck::SetDeckOwner(AActor* owner)
{
	DeckOwner = owner;
}

void UComponentRPG_BuffDeck::Init()
{
	for (int i = 0; i < (uint8)BuffType::BuffType_Max; ++i)
	{
		BuffArrayByType.Add(NewObject<UBuffArrayByType>(this));
	}
}

void UComponentRPG_BuffDeck::AddBuffToDeck(AComponentRPG_BuffActor* buff)
{	
	buff->SetOwner(DeckOwner);
	BuffArray.Add(buff);
	BuffArrayByType[(uint8)buff->GetBuffType()]->BuffArray.Add(buff);

	//buff->StartTick();
}

//UComponentRPG_SkillComponent* UComponentRPG_SkillComponentDeck::GetTargetingComponent()
//{
//	if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_Targeting]->SkillComponentArray[0] != nullptr)
//		return SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_Targeting]->SkillComponentArray[0];
//	else
//		return nullptr;
//}
//
//UComponentRPG_SkillComponent* UComponentRPG_SkillComponentDeck::GetMoveCommandComponent()
//{
//	if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_MoveCommand]->SkillComponentArray[0] != nullptr)
//		return SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_MoveCommand]->SkillComponentArray[0];
//	else
//		return nullptr;
//}
//
/////// Helper function
//int32 UComponentRPG_SkillComponentDeck::GetSumAdditionalNumTarget()
//{
//	int32 result = 0;
//	for (int32 i = 0; i < SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray.Num(); ++i)
//	{
//		if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray[i]->GetIsSelected())
//			result += Cast<URPG_SkillComponent_PlusNumTarget>(SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalTarget]->SkillComponentArray[i])->AdditionalNumTarget;
//	}
//
//	return result;
//}
//
//float UComponentRPG_SkillComponentDeck::GetSumAdditionalAreaSize()
//{
//	float result = 0;
//	for (int32 i = 0; i < SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray.Num(); ++i)
//	{
//		if (SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray[i]->GetIsSelected())
//			result += Cast<URPG_SkillComponent_PlusAreaSize>(SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_AdditionalAreaSize]->SkillComponentArray[i])->AdditionalAreaSize;
//	}
//
//	return result;
//}

