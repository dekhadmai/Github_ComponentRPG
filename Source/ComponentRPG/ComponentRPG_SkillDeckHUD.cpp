// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_SkillDeckHUD.h"



void UComponentRPG_SkillDeckHUD::SetSkillDeck(UComponentRPG_SkillComponentDeck* deck)
{
	if (deck != nullptr)
		SkillDeck = deck;

	RefreshWidgetData();
}

void UComponentRPG_SkillDeckHUD::SetVBox(UPanelWidget* skilltarget_vbox, UPanelWidget* skilllist_vbox)
{
	SkillTargetingWidget = skilltarget_vbox;
	SkillListWidget = skilllist_vbox;
	
	RefreshWidgetData();
}

void UComponentRPG_SkillDeckHUD::RefreshWidgetData()
{
	Super::RefreshWidgetData();

	SkillTargetingWidget->ClearChildren();
	SkillListWidget->ClearChildren();

	if (SkillTargetingWidget != nullptr && SkillListWidget != nullptr && SkillDeck != nullptr)
	{
		for (int32 i = 0; i < SkillDeck->SkillComponentArray.Num(); ++i)
		{
			// create widget
			UComponentRPG_SkillButtonWidget* tmpButton;
			TSubclassOf<UUserWidget> tmpButtonWidget;
			if ( SkillDeck->SkillComponentArray[i]->GetSkillComponentType() != SkillComponentType::SkillComponentType_Targeting)
			{
				if (SkillDeck->SkillComponentArray[i]->GetSkillComponentType() == SkillComponentType::SkillComponentType_MoveCommand)
				{
					//tmpButtonWidget = MoveCommandButtonWidget;
					tmpButtonWidget = ButtonWidget;
				}
				else
				{
					tmpButtonWidget = ButtonWidget;
				}

				tmpButton = CreateWidget<UComponentRPG_SkillButtonWidget>(GetWorld(), tmpButtonWidget);
				tmpButton->SetSkillComponent(SkillDeck->SkillComponentArray[i]);
				tmpButton->SetController(myController);
				SkillListWidget->AddChild(tmpButton);
			}
			else
			{
				tmpButton = CreateWidget<UComponentRPG_TargetingWidget>(GetWorld(), TargetingButtonWidget);
				tmpButton->SetSkillComponent(SkillDeck->SkillComponentArray[i]);
				tmpButton->SetController(myController);
				SkillTargetingWidget->AddChild(tmpButton);
			}
		}
	}
}