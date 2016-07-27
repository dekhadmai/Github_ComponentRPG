// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPG_UserWidget.h"
#include "SkillComponentArrayByType.h"
#include "ComponentRPG_SkillComponent.generated.h"

/**
*
*/

struct FSkillComponentCustomization;
class UComponentRPG_SkillComponent;
class AComponentRPG_BuffActor;

UENUM()
enum class TargetingType : uint8
{
	TargetingType_None,
	TargetingType_Missile,
	TargetingType_AOE,
	TargetingType_Melee
};

UENUM()
enum class TargetSideType : uint8
{
	TargetSideType_None,
	TargetSideType_Enemy,
	TargetSideType_Friend,
	TargetSideType_Both
};

UENUM()
enum class TargetType : uint8
{
	TargetType_None,
	TargetType_Character,
	TargetType_Ground
};

USTRUCT()
struct FTargetingInstruction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TargetingType targeting_type;

	UPROPERTY()
		TargetSideType target_side;

	UPROPERTY()
		TargetType target_type;

	UPROPERTY()
		int32 num_of_target;

	UPROPERTY()
		float area_size;
};

UENUM()
enum class DamageType : uint8
{
	DamageType_None,
	DamageType_NoElement,
	DamageType_Fire,
	DamageType_Ice,
	DamageType_Lightning,
	DamageType_Poison,
	DamageType_Heal
};

UENUM()
enum class ChanceOnHitEffect : uint8
{
	ChanceOnHitEffect_None,
	ChanceOnHitEffect_Freeze,
	ChanceOnHitEffect_Burn,
	ChanceOnHitEffect_Stun,
	ChanceOnHitEffect_Critical,
	ChanceOnHitEffect_Interrupt
};

USTRUCT()
struct FChanceOnHitStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float PercentChance;

	UPROPERTY()
		ChanceOnHitEffect Effect;

	UPROPERTY()
		float EffectDuration;
};

UENUM()
enum class SkillComponentType : uint8
{
	SkillComponentType_None, // shouldn't happen
	SkillComponentType_MoveCommand,
	SkillComponentType_DefenseCommand,
	SkillComponentType_Execute,
	SkillComponentType_Targeting,
	SkillComponentType_AdditionalTarget,
	SkillComponentType_AdditionalAreaSize,
	SkillComponentType_DamageType,
	SkillComponentType_Damage,
	SkillComponentType_ChanceOnHit,
	SkillComponentType_StatBoost,
	SkillComponentType_ChangeToBuff,
	SkillComponentType_CharSpecialProperty, //Uninterruptible
	SkillComponentType_Max
};

UCLASS()
class COMPONENTRPG_API UComponentRPG_SkillComponent : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
		bool isSelected; // selected by player (indicated by checkbox), search for SkillComponent->isSelected

public:
	UComponentRPG_SkillComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags);

	UPROPERTY(Replicated)
		UObject* ComponentOwner;

	virtual void SetOwner(UObject* owner);

	UPROPERTY(Replicated)
		SkillComponentType m_SkillComponentType;

	UPROPERTY(Replicated)
		bool bIsActive;

	UPROPERTY(Replicated)
		bool bHasTickEffect;

	// helper functions
	virtual bool GetIsSelected();
	virtual bool GetIsEnabled();
	virtual void SetIsSelected(bool val);
	bool IsActive();
	SkillComponentType GetSkillComponentType();

	float CastRateModifier;
	virtual float GetSkillComponentCastRateModifier();

	// state functions
	virtual void Init(void* obj = NULL);
	virtual void ComponentSetup(FSkillComponentCustomization custom);
	virtual void Activate();
	virtual void OnTickEffect(float deltaTime);
	virtual void Deactivate();

	virtual void ResetDefaultValue();
	virtual void RandomizeComponent();

	virtual void BuffActorModification(AComponentRPG_BuffActor* buff_actor);

	///// specific component functions
	// targeting component
	virtual void ClearAllTarget();
	virtual void ServerClearAllTarget();

	virtual bool GetIsSelectingTarget();
	virtual void StartSelectingTarget();
	virtual void ShowCurrentTargetUI();
	virtual void StopSelectingTarget();
	virtual TargetType GetCurrentTargetType();
	virtual void AddTargetCharacter(ACharacter* targetChar, UComponentRPG_UserWidget* translucentHUD);
	virtual void AddTargetCharacterToArray(ACharacter* targetChar);
	virtual void AddTargetLocation(FVector targetLoc, UComponentRPG_UserWidget* translucentHUD);
	virtual void AddTargetLocationToArray(FVector targetLoc);

	virtual void SetTargetingType(TargetingType val);
	virtual void SetTargetSideType(TargetSideType val);
	virtual void SetTargetType(TargetType val);
	virtual TargetingType GetTargetingType();
	virtual TargetSideType GetTargetSideType();
	virtual TargetType GetTargetType();

	virtual FString GetSkillComponentDescription();
	virtual FString GetSkillComponentDescriptionTooltip();
	virtual FString GetSkillComponentBasicTooltipString();
	//virtual void GetSkillComponentIcon();
};
