// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ComponentRPG_SkillComponent.h"
#include "ComponentRPG_Skill.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMPONENTRPG_API UComponentRPG_Skill : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentRPG_Skill();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// dont forget to tick skill comp
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// skill components, use for setup skill properties
	UPROPERTY()
		TArray<UComponentRPG_SkillComponent*> SkillComponentArray;

	// skill components, skill description (skill builder)
	// TArray<UComponentRPG_SkillComponent_damagetype*> SkillComponentArray;
	
	// skill components, skill gameplay effect (burn, freeze, etc.)


	// skill properties
	UPROPERTY()
	UObject* SkillOwner;

	// skill component related
	void Init();
	void AddSkillComponent(UComponentRPG_SkillComponent* comp);
	//virtual void Activate(bool bReset = false) override;
	void OnTickEffect(float deltaTime);
	//virtual void Deactivate() override;
};
