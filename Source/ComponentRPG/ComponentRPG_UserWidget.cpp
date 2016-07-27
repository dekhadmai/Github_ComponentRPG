// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPG_UserWidget.h"

void UComponentRPG_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*UTextBlock* tmp = Cast<UTextBlock>(GetWidgetFromName("NewgameTxt"));
	tmp->SetText(FText( NSLOCTEXT( "Your Namespace", "Your Key", "Your Text" )  ));*/
}

void UComponentRPG_UserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UComponentRPG_UserWidget::SetController(AController* controller)
{
	if (controller != nullptr)
		myController = controller;
}

void UComponentRPG_UserWidget::SetWidgetOwner(UObject* owner)
{
	if (owner != nullptr)
		myOwner = owner;
}

void UComponentRPG_UserWidget::RefreshWidgetData()
{
	if (myController == nullptr)
		return;
}

UComponentRPG_UserWidget* UComponentRPG_UserWidget::GetTooltipWidget()
{
	UComponentRPG_UserWidget* tmpWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CustomTooltipWidget);
	
	return tmpWidget;
}

FText UComponentRPG_UserWidget::GetCustomTooltipText()
{
	return FText::FromString(CustomTooltipString);
}

void UComponentRPG_UserWidget::SetHUDPos()
{

}

void UComponentRPG_UserWidget::SetHUDPos(FVector worldPos)
{
	FVector2D result;
	Cast<AComponentRPG_PlayerController>(myController)->ProjectWorldLocationToScreen(worldPos, result);
	SetPositionInViewport(result);
}