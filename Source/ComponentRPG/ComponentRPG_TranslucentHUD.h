// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPGCharacter.h"
#include "ComponentRPG_PlayerController.h"
#include "ComponentRPG_UserWidget.h"
#include "Image.h"
#include "VerticalBox.h"
#include "ComponentRPG_TranslucentHUD.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_TranslucentHUD : public UComponentRPG_UserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UImage* IndicatorImage;

	UPROPERTY()
		UVerticalBox* IndicatorVBox;

	UPROPERTY()
		FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
		UTexture2D* CircleTexture;

	UPROPERTY()
		FLinearColor IndicatorColor;

	float TileSize;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void SetHUDPos() override;
	virtual void SetHUDPos(FVector worldPos) override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		virtual void SetWidgetOwner(UObject* owner) override;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetIndicatorImage(UImage* img);

	void SetSquareIndicator();
	void SetCircleIndicator();

	void SetIndicatorSize(float width, float height);
	void SetIndicatorColor(FLinearColor val);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FLinearColor GetIndicatorColor();

};
