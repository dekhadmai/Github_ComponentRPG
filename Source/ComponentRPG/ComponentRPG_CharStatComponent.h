// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ComponentRPG_CharStatComponent.generated.h"


USTRUCT()
struct FCharacterStatPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float STATPOINT_DAMAGE; // +atk%

	UPROPERTY()
	float STATPOINT_EFFECTIVENESS; // +secondary effect

	UPROPERTY()
	float STATPOINT_DEFENSE; // %dmg reduction

	UPROPERTY()
	float STATPOINT_RESISTANCE; // %secondary effect reduction

	UPROPERTY()
	float STATPOINT_SPEED; // speed to fill action bar

	UPROPERTY()
	float STATPOINT_RECOVERY; // regen %

	UPROPERTY()
	float STATPOINT_MAXBAR; // max hp,mp
};

USTRUCT()
struct FCharacterStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float STAT_DAMAGE; // +atk%

	UPROPERTY()
	float STAT_EFFECTIVENESS; // +secondary effect

	UPROPERTY()
	float STAT_DEFENSE; // %dmg reduction

	UPROPERTY()
	float STAT_RESISTANCE; // %secondary effect reduction

	UPROPERTY()
	float STAT_SPEED; // speed to fill action bar

	UPROPERTY()
	float STAT_RECOVERY; // regen %

	UPROPERTY()
	float STAT_MAXBAR; // max hp,mp
};

USTRUCT()
struct FCharacterBaseStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float BASE_HP;
	
	UPROPERTY()
	float BASE_MP;

	UPROPERTY()
	float BASE_DAMAGE;

	UPROPERTY()
	float BASE_EFFECTIVENESS;

	UPROPERTY()
	float BASE_DEFENSE;

	UPROPERTY()
	float BASE_RESISTANCE;

	UPROPERTY()
	float BASE_SPEED;

	UPROPERTY()
	float BASE_RECOVERY;

	UPROPERTY()
	float BASE_ACTIONBAR;

	UPROPERTY()
	float BASE_CASTBAR;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMPONENTRPG_API UComponentRPG_CharStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentRPG_CharStatComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/*virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}*/

	void ComponentInit(FCharacterStatPoint profile_stat, bool bRandomStat);
	void RefreshBaseStats();

	float BaseCastingSpeed;
	float BaseDamage;

	UPROPERTY(Replicated)
	FCharacterStatPoint StatPoints;

	UPROPERTY(Replicated)
	FCharacterStatPoint CurrentStatPoints;

	//UPROPERTY(Replicated)
	//FCharacterStat Stats;

	UPROPERTY(Replicated)
	FCharacterBaseStat BaseStats; // snapshot for calculation

	UPROPERTY(Replicated)
	FCharacterBaseStat CurrentStats;
};