// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_SkillButtonWidget.h"



void UComponentRPG_SkillButtonWidget::SetSkillComponent(UComponentRPG_SkillComponent* comp)
{
	if (comp != nullptr)
		SkillComponent = comp;
}

FText UComponentRPG_SkillButtonWidget::GetText()
{
	if (SkillComponent != nullptr)
	{
		FString result = SkillComponent->GetSkillComponentDescription();
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("No SkillComponent");
	}
}

UComponentRPG_UserWidget* UComponentRPG_SkillButtonWidget::GetTooltipWidget()
{
	UComponentRPG_UserWidget* tmpWidget = Super::GetTooltipWidget();
	tmpWidget->CustomTooltipString = SkillComponent->GetSkillComponentDescriptionTooltip();
	
	return tmpWidget;
}

void UComponentRPG_SkillButtonWidget::GetIcon()
{

}

ECheckBoxState UComponentRPG_SkillButtonWidget::GetIsSelected()
{
	if (!SkillComponent)
		return ECheckBoxState::Unchecked;

	if (SkillComponent->GetIsSelected())
		return ECheckBoxState::Checked;
	else
		return ECheckBoxState::Unchecked;
}

bool UComponentRPG_SkillButtonWidget::GetIsEnabled()
{
	return SkillComponent->GetIsEnabled();
}

void UComponentRPG_SkillButtonWidget::OnButtonClicked()
{
	if (myController != nullptr)
	{
		bool tmpBool = !SkillComponent->GetIsSelected();
		Cast<AComponentRPG_PlayerController>(myController)->SkillComponent_SetIsSelected(SkillComponent, tmpBool);

		//if (SkillComponent->GetSkillComponentType() == SkillComponentType::SkillComponentType_Execute)
		//{
		//	if (tmpBool)
		//	{
		//		if (Cast<AComponentRPGCharacter>(SkillComponent->ComponentOwner) != nullptr)
		//		{
		//			if (Cast<AComponentRPGCharacter>(SkillComponent->ComponentOwner)->SkillComponentDeck != nullptr)
		//			{
		//				Cast<AComponentRPGCharacter>(SkillComponent->ComponentOwner)->SkillComponentDeck->GetTargetingComponent()->ClearAllTarget();
		//				Cast<AComponentRPGCharacter>(SkillComponent->ComponentOwner)->SkillComponentDeck->GetTargetingComponent()->ServerClearAllTarget();
		//			}
		//		}
		//	}
		//}
	}
}