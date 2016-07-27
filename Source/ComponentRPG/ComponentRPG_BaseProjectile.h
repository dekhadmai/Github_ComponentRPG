// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "ComponentRPG_CharStatComponent.h"
#include "ComponentRPG_SkillComponentDeck.h"
#include "ComponentRPG_BaseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API AComponentRPG_BaseProjectile : public APaperCharacter
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ProjectileAnimation;
	class UPaperFlipbook* ProjectileAOEAnimation;
	
public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(Replicated)
		UComponentRPG_CharStatComponent* CharStatComponent;

	UPROPERTY(Replicated)
		UComponentRPG_SkillComponentDeck* SkillComponentDeck;

	UPROPERTY(Replicated)
		uint8 TeamIndex;

	UPROPERTY(Replicated)
		AComponentRPGCharacter* ProjOwner;

	AComponentRPG_BaseProjectile();

	virtual void PostInitializeComponents() override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	void UpdateCharacter();

	UPROPERTY()
		bool bSkipOverlapCheck;
	UPROPERTY()
		bool bDestroySelfWhenReachDestination;
	UPROPERTY(Replicated)
		FVector DestPosition;
	UPROPERTY(Replicated)
		AActor* DestActor;
	float HomingMagnitude;

	void SetProjectileAnim(int animIdx);
	void SetTargetingTypeAOE();

	UFUNCTION()
		void AddDuplicateSkillComponentToDeck(UComponentRPG_SkillComponent* skill_comp);

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool SpawnAreaBuffWhenDestroyed;

	UFUNCTION()
		virtual bool DestroyProjectile();

	void ProjectileCommonSetup(AComponentRPG_BaseProjectile* tmp_proj, AComponentRPGCharacter* tmp_char);

	///// InitialSpeed
	UPROPERTY(ReplicatedUsing = OnRep_myInitialSpeed)
		float myInitialSpeed;

	UFUNCTION()
		void OnRep_myInitialSpeed();

	UFUNCTION()
		void SetInitialSpeed(float val);

	///// LifeSpan
	UPROPERTY(Replicated)
		bool bUseLifeSpan;
	UPROPERTY(Replicated)
		float LifeSpan;
	UFUNCTION()
		virtual void SetActorLifeSpan( float InLifespan );
	UFUNCTION()
		virtual float GetLifeSpan() const override;
	///// end of LifeSpan

	UFUNCTION()
		virtual void SetActorSizeMultiplier(float size_mult);

	///// CustomTimeDilation
	UPROPERTY(ReplicatedUsing = OnRep_myCustomTimeDilation)
		float myCustomTimeDilation;

	UFUNCTION()
		void OnRep_myCustomTimeDilation();

	UFUNCTION()
		void SetCustomTimeDilation(float val);
	///// end of CustomTimeDilation
};
