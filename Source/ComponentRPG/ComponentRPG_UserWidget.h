// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ComponentRPG_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AController* myController;

	UPROPERTY()
	UObject* myOwner;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual void SetController(AController* controller);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual void SetWidgetOwner(UObject* owner);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual void RefreshWidgetData();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual UComponentRPG_UserWidget* GetTooltipWidget();

	UPROPERTY()
		FString CustomTooltipString;
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetCustomTooltipText();

	virtual void SetHUDPos();
	virtual void SetHUDPos(FVector worldPos);
};
