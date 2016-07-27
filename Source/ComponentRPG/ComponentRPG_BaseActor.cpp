// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_BaseActor.h"


// Sets default values
AComponentRPG_BaseActor::AComponentRPG_BaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AComponentRPG_BaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComponentRPG_BaseActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

