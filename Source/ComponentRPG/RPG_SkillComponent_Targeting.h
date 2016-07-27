// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_SkillComponent.h"
#include "RPG_SkillComponent_Targeting.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API URPG_SkillComponent_Targeting : public UComponentRPG_SkillComponent
{
	GENERATED_BODY()

public:

	FLinearColor IndicatorColor;

	URPG_SkillComponent_Targeting(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated)
		FTargetingInstruction targeting_instruction;

	UPROPERTY(Replicated)
		bool isSelectingTarget;

	UPROPERTY(Replicated)
		bool canPickTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UComponentRPG_UserWidget> TranslucentTemplate_UI;

	UPROPERTY(ReplicatedUsing = CheckStopTargeting)
		TArray<ACharacter*> TargetCharacterArray;
	UPROPERTY()
		TArray<UComponentRPG_UserWidget*> TargetCharacterArray_UI;

	UPROPERTY(ReplicatedUsing = CheckStopTargeting)
		TArray<FVector> TargetLocationArray;
	UPROPERTY()
		TArray<UComponentRPG_UserWidget*> TargetLocationArray_UI;

	virtual void Init(void* obj = NULL) override;
	virtual FString GetSkillComponentDescription() override;
	
	virtual void ClearAllTarget() override;
	void ServerClearAllTarget();
	virtual void SetClientClearTargetingComponent();

	void EnableCanPickTarget();
	virtual bool GetIsSelectingTarget() override;
	virtual void StartSelectingTarget() override;
	virtual void ShowCurrentTargetUI() override;
	virtual void StopSelectingTarget() override;
	virtual TargetType GetCurrentTargetType() override;
	
	virtual void AddTargetCharacter(ACharacter* targetChar, UComponentRPG_UserWidget* translucentHUD) override;
	virtual void AddTargetCharacterToArray(ACharacter* targetChar) override;
	void ServerAddTargetCharacterToArray(ACharacter* targetChar);

	virtual void AddTargetLocation(FVector targetLoc, UComponentRPG_UserWidget* translucentHUD) override;
	virtual void AddTargetLocationToArray(FVector targetLoc) override;
	void ServerAddTargetLocationToArray(FVector targetLoc);

	UFUNCTION()
		void CheckStopTargeting();

	virtual void SetTargetingType(TargetingType val) override;
	virtual void SetTargetSideType(TargetSideType val) override;
	virtual void SetTargetType(TargetType val) override;

	virtual TargetingType GetTargetingType() override;
	virtual TargetSideType GetTargetSideType() override;
	virtual TargetType GetTargetType() override;

	virtual void ResetDefaultValue() override;
	virtual void RandomizeComponent() override;

	virtual int32 GetNumTarget();
	virtual float GetAreaSize();

	int32 GetSumAdditionalNumTarget();
	float GetSumAdditionalAreaSize();
	
};