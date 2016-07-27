// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "ComponentRPG_SkillComponent.h"
#include "ComponentRPG_BuffActor.generated.h"

UENUM()
enum class BuffType : uint8
{
	BuffType_None, // shouldn't happen
	BuffType_Damage,
	BuffType_Status,
	BuffType_StatBoost,
	BuffType_Max
};

UCLASS()
class COMPONENTRPG_API AComponentRPG_BuffActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComponentRPG_BuffActor();

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* BuffAnimation;

	UPROPERTY(Replicated)
		bool bIsInstantBuff;

	///// Buff property
	UPROPERTY(Replicated)
		BuffType m_BuffType;

	UPROPERTY(Replicated)
		float BuffDamage;

	UPROPERTY(Replicated)
		DamageType BuffDamageType;

	///// end of Buff property

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void StartTick();
	virtual bool DestroyBuffActor();
	
	/////
	UFUNCTION()
		BuffType GetBuffType();

	UFUNCTION()
		FString GetBuffDescription();

	///// LifeSpan
	UPROPERTY(Replicated)
		bool bUseLifeSpan;
	UPROPERTY(Replicated)
		float OriginalLifeSpan;
	UPROPERTY(Replicated)
		float LifeSpan;
	UFUNCTION()
		virtual void SetActorLifeSpan( float InLifespan );
	UFUNCTION()
		virtual float GetLifeSpan() const override;
	///// end of LifeSpan

	///// CustomTimeDilation
	UPROPERTY(ReplicatedUsing = OnRep_myCustomTimeDilation)
		float myCustomTimeDilation;

	UFUNCTION()
		void OnRep_myCustomTimeDilation();

	UFUNCTION()
		void SetCustomTimeDilation(float val);
	///// end of CustomTimeDilation
};
