// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ComponentRPG_CharStatComponent.h"
#include "ComponentRPG_SaveProfile.generated.h"

USTRUCT()
struct FSkillComponentCustomization
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		uint32 SkillComponentId;

	UPROPERTY()
		uint32 Customize_int1;

	UPROPERTY()
		uint32 Customize_int2;

	UPROPERTY()
		uint32 Customize_int3;

	UPROPERTY()
		uint32 Customize_int4;

	UPROPERTY()
		float Customize_float1;

	UPROPERTY()
		float Customize_float2;

	UPROPERTY()
		float Customize_float3;

	UPROPERTY()
		float Customize_float4;
};

USTRUCT()
struct FCharacterProfile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		uint32 CharID;

	UPROPERTY()
		uint32 CharIndex;

	UPROPERTY()
		FString CharName;

	UPROPERTY()
		uint32 CharLevel;

	UPROPERTY()
		FCharacterStatPoint CharStatPoint;

	UPROPERTY()
		TArray<FSkillComponentCustomization> CharSkillComponentIndexArray;
};

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UComponentRPG_SaveProfile : public UObject
{
	GENERATED_BODY()
	
public:

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UPROPERTY(Replicated)
		TArray<FCharacterProfile> CharProfileArray;
	
};
