// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_SkillComponent.h"

UComponentRPG_SkillComponent::UComponentRPG_SkillComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CastRateModifier = 0.85f;

	//Init();
}

bool UComponentRPG_SkillComponent::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	return false;
}

void UComponentRPG_SkillComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UComponentRPG_SkillComponent, m_SkillComponentType);
	DOREPLIFETIME(UComponentRPG_SkillComponent, isSelected);
	DOREPLIFETIME(UComponentRPG_SkillComponent, bIsActive);
	DOREPLIFETIME(UComponentRPG_SkillComponent, bHasTickEffect);
	DOREPLIFETIME(UComponentRPG_SkillComponent, ComponentOwner);
}

void UComponentRPG_SkillComponent::SetOwner(UObject* owner)
{
	ComponentOwner = owner;
}

void UComponentRPG_SkillComponent::Init(void* obj)
{
	ResetDefaultValue();
}

void UComponentRPG_SkillComponent::ComponentSetup(FSkillComponentCustomization custom)
{

}

bool UComponentRPG_SkillComponent::GetIsSelected()
{
	return isSelected;
}

bool UComponentRPG_SkillComponent::GetIsEnabled()
{
	return true;
}

void UComponentRPG_SkillComponent::SetIsSelected(bool val)
{
	isSelected = val;
}

bool UComponentRPG_SkillComponent::IsActive()
{
	return bIsActive;
}

SkillComponentType UComponentRPG_SkillComponent::GetSkillComponentType()
{
	return m_SkillComponentType;
}

float UComponentRPG_SkillComponent::GetSkillComponentCastRateModifier()
{
	return CastRateModifier;
}


void UComponentRPG_SkillComponent::Activate()
{
	bIsActive = true;
}

void UComponentRPG_SkillComponent::OnTickEffect(float deltaTime)
{

}

void UComponentRPG_SkillComponent::Deactivate()
{
	bIsActive = false;
}

void UComponentRPG_SkillComponent::ResetDefaultValue()
{

}

void UComponentRPG_SkillComponent::RandomizeComponent()
{

}

void UComponentRPG_SkillComponent::BuffActorModification(AComponentRPG_BuffActor* buff_actor)
{

}

TargetingType UComponentRPG_SkillComponent::GetTargetingType()
{
	return TargetingType::TargetingType_None;
}

TargetSideType UComponentRPG_SkillComponent::GetTargetSideType()
{
	return TargetSideType::TargetSideType_None;
}

TargetType UComponentRPG_SkillComponent::GetTargetType()
{
	return TargetType::TargetType_None;
}

void UComponentRPG_SkillComponent::ClearAllTarget()
{
}

void UComponentRPG_SkillComponent::ServerClearAllTarget()
{
}

bool UComponentRPG_SkillComponent::GetIsSelectingTarget()
{
	return false;
}

void UComponentRPG_SkillComponent::StartSelectingTarget()
{

}

void UComponentRPG_SkillComponent::ShowCurrentTargetUI()
{

}

void UComponentRPG_SkillComponent::StopSelectingTarget()
{

}

TargetType UComponentRPG_SkillComponent::GetCurrentTargetType()
{
	return TargetType::TargetType_None;
}

void UComponentRPG_SkillComponent::AddTargetCharacter(ACharacter* targetChar, UComponentRPG_UserWidget* translucentHUD)
{

}

void UComponentRPG_SkillComponent::AddTargetCharacterToArray(ACharacter* targetChar)
{

}

void UComponentRPG_SkillComponent::AddTargetLocation(FVector targetLoc, UComponentRPG_UserWidget* translucentHUD)
{

}

void UComponentRPG_SkillComponent::AddTargetLocationToArray(FVector targetLoc)
{

}

void UComponentRPG_SkillComponent::SetTargetingType(TargetingType val)
{

}

void UComponentRPG_SkillComponent::SetTargetSideType(TargetSideType val)
{

}

void UComponentRPG_SkillComponent::SetTargetType(TargetType val)
{

}

FString UComponentRPG_SkillComponent::GetSkillComponentDescription()
{
	return "Base Description";
}

FString UComponentRPG_SkillComponent::GetSkillComponentDescriptionTooltip()
{
	return "Base Description Tooltip";
}

FString UComponentRPG_SkillComponent::GetSkillComponentBasicTooltipString()
{
	FString result = "";
	result += "Cast Rate Modifier : " + FString::Printf(TEXT("%.2f"), CastRateModifier * 100) + "%\n";

	return result;
}