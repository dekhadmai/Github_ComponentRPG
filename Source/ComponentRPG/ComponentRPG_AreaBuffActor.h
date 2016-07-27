// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_BaseProjectile.h"
#include "ComponentRPG_AreaBuffActor.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API AComponentRPG_AreaBuffActor : public AComponentRPG_BaseProjectile
{
	GENERATED_BODY()
	
public:
	
	TArray<AComponentRPG_BuffActor*> BuffApplyArray;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
		virtual bool DestroyProjectile();
};
