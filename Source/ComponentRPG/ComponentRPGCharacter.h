// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "ComponentRPG_UserWidget.h"
#include "ComponentRPG_CharacterManager.h"
#include "ComponentRPG_Skill.h"
#include "ComponentRPG_CharStatComponent.h"
#include "ComponentRPG_SkillComponentDeck.h"
#include "ComponentRPG_BuffDeck.h"
#include "ComponentRPG_TestRep.h"
#include "ComponentRPGCharacter.generated.h"

// This class is the default character for ComponentRPG, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

class UTextRenderComponent;

UCLASS(config=Game)
class AComponentRPGCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	///// HUD
    /** The widget instance that we are using as our menu. */
    UPROPERTY()
		UComponentRPG_UserWidget* CurrentWidget;

public:
	AComponentRPGCharacter();

	virtual void PostInitializeComponents() override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void OnHitEvent(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual bool DestroyCharacter();
	//bool Destroy(bool bNetForce = false, bool bShouldModifyLevel = true );

	UPROPERTY(Replicated)
		uint8 TeamIndex;

	UPROPERTY(Replicated)
		bool bWaitingForAction;

	///// ActorComponents
	//UPROPERTY(Replicated)
	//	UComponentRPG_CharacterManager* CharacterManager;

	//UPROPERTY(Replicated)
	//	UComponentRPG_Skill* CharacterSkill;

	UPROPERTY(Replicated)
		UComponentRPG_CharStatComponent* CharStatComponent;

	UPROPERTY(Replicated)
		UComponentRPG_SkillComponentDeck* SkillComponentDeck;

	UPROPERTY(Replicated)
		UComponentRPG_BuffDeck* BuffDeck;

	UPROPERTY(Replicated)
		UComponentRPG_TestRep* testRep;
	///// end of ActorComponents

	/** Called for side to side input */
	void MoveRight(float Value);

	// setup character
	void SetupCharacter();

	// create widget on client
	UFUNCTION(Reliable, Client)
		void ClientCreateCharacterWidget();

	void CreateCharacterWidget();

	void CheckCurrentWidgetTimer();

	///// character function
	UPROPERTY(Replicated)
		int32 ConsecutiveCastInterruptionCount;

	virtual void CharTakeDamage(AComponentRPG_BuffActor* DamageCauser, float damage_amount);
	///// end of character function

	///// Animation Related
	UPROPERTY(Replicated)
		bool IsPlayingCustomAnim;

	UFUNCTION()
		void PlayCustomAnim(class UPaperFlipbook* anim);
	
	UFUNCTION()
		void OnCustomAnimFinish();
	///// end of Animation Related

	///// casting spell
	UPROPERTY(Replicated)
		bool IsCastingSkill;
	UFUNCTION()
		void StartCastingSkill();

	///// move to function
	void FakeTickController();
	
	UPROPERTY(Replicated)
		FVector EndLocation;
	UPROPERTY(Replicated)
		AActor* MoveToActor;
	UPROPERTY()
		float EndLocationRadiusCheck;
	UPROPERTY(Replicated)
		bool IsMovingToLocation;
	void SetMoveToLocation(FVector location);
	void SetMoveToActor(AActor* targetActor);

	UPROPERTY(Replicated)
		bool IsMeleeAttacking;
	void SetIsMeleeAttacking(bool val);
	///// end of move to function

	///// CustomTimeDilation
	UPROPERTY(ReplicatedUsing = OnRep_myCustomTimeDilation)
		float myCustomTimeDilation;

	UFUNCTION()
		void OnRep_myCustomTimeDilation();

	UFUNCTION()
		void SetCustomTimeDilation(float val);
	///// end of CustomTimeDilation

	///// clear TargetingComponent horrible hack
	UPROPERTY(ReplicatedUsing = OnRep_ClientClearTargetingComponent)
		bool bClientClearTargetingComponent;

	UFUNCTION()
		void OnRep_ClientClearTargetingComponent();

	UFUNCTION()
		void SetClientClearTargetingComponent(bool val);

	UFUNCTION()
		void ResetClientClearTargetingComponent();
	///// end of clean TargetingComponent horrible hack

	/////// clear MovementComponent horrible hack
	//UPROPERTY(ReplicatedUsing = OnRep_ClientClearMovementComponent)
	//	bool bClientClearMovementComponent;

	//UFUNCTION()
	//	void OnRep_ClientClearMovementComponent();

	//UFUNCTION()
	//	void SetClientClearMovementComponent(bool val);

	//UFUNCTION()
	//	void ResetClientClearMovementComponent();
	/////// end of clean MovementComponent horrible hack
	
	/*void HttpRequest();
	void HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);*/

	UPROPERTY()
	FString UI_Response;
};
