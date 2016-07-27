// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_Skill.h"

// Sets default values for this component's properties
UComponentRPG_Skill::UComponentRPG_Skill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UComponentRPG_Skill::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
}


// Called every frame
void UComponentRPG_Skill::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	OnTickEffect(DeltaTime);
}

void UComponentRPG_Skill::Init()
{
	// reset skill
	
}

void UComponentRPG_Skill::AddSkillComponent(UComponentRPG_SkillComponent* comp)
{
	SkillComponentArray.Add(comp);
}

void UComponentRPG_Skill::OnTickEffect(float deltaTime)
{
	for (int32 i = 0; i < SkillComponentArray.Num(); ++i)
	{
		SkillComponentArray[i]->OnTickEffect(deltaTime);
	}
}

