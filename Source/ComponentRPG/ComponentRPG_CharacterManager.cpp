// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_CharacterManager.h"
#include "ComponentRPGCharacter.h"


// Sets default values for this component's properties
UComponentRPG_CharacterManager::UComponentRPG_CharacterManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
}


// Called when the game starts
void UComponentRPG_CharacterManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


// Called every frame
void UComponentRPG_CharacterManager::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


void UComponentRPG_CharacterManager::SetupCharacter()
{
	// add other ActorComponents
	/*AComponentRPGCharacter* myCharacter = Cast<AComponentRPGCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	myCharacter->SetupCharacter();*/
}
