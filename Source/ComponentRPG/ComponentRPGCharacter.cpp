// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ComponentRPG.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_PlayerController.h"
#include "PaperFlipbookComponent.h"
#include "ComponentRPGGameMode.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_CharacterHUD.h"
#include "Components/TextRenderComponent.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// AComponentRPGCharacter

AComponentRPGCharacter::AComponentRPGCharacter()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackAnimationAsset;
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Female_WalkRight.Female_WalkRight"))
			, IdleAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Female_Idle.Female_Idle"))
			, AttackAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Female_Attack.Female_Attack"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	bAlwaysRelevant = true;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	AttackAnimation = ConstructorStatics.AttackAnimationAsset.Get();
	GetSprite()->SetFlipbook(IdleAnimation);
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &AComponentRPGCharacter::OnCustomAnimFinish);
	IsPlayingCustomAnim = false;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(20.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AComponentRPGCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AComponentRPGCharacter::OnEndOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AComponentRPGCharacter::OnHitEvent);

	// Configure character movement
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->AirControl = 1.0f; //0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->AttachTo(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	///// variable init
	EndLocationRadiusCheck = 5.0f;
	CurrentWidget = nullptr;
	TeamIndex = -1;
	myCustomTimeDilation = 1.0f;
	bClientClearTargetingComponent = false;

	///// actor component init
	CharStatComponent = CreateDefaultSubobject<UComponentRPG_CharStatComponent>(TEXT("CharStatComponent"));
	CharStatComponent->SetNetAddressable();
	CharStatComponent->SetIsReplicated(true);

	//CharacterManager = CreateDefaultSubobject<UComponentRPG_CharacterManager>(TEXT("CharacterManager"));
	//CharacterSkill = CreateDefaultSubobject<UComponentRPG_Skill>(TEXT("Skill"));
}

void AComponentRPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		testRep = NewObject<UComponentRPG_TestRep>(this);
		testRep->Init();
		//AComponentRPG_BuffActor* tmpBuff;
		//tmpBuff = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->SpawnBuffActor();
		//testRep->AddBuffActor(tmpBuff);

		SkillComponentDeck = NewObject<UComponentRPG_SkillComponentDeck>(this);
		SkillComponentDeck->SetDeckOwner(this);
		SkillComponentDeck->Init();

		BuffDeck = NewObject<UComponentRPG_BuffDeck>(this);
		BuffDeck->SetDeckOwner(this);
		BuffDeck->Init();

		/////// Add SkillComp to SkillDeck
		//AComponentRPG_GameState* tmpGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
		//if (tmpGameState)
		//{
		//	// move comp
		//	SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(tmpGameState->MainSkillComponentList[0]->GetDefaultObject(), tmpGameState->MainSkillComponentList[0]->GetDefaultObject()->GetOuter())) );

		//	// configurable comp
		//	for (int i = 0; i < tmpGameState->AllSkillComponentList.Num(); ++i)
		//	{
		//		SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(tmpGameState->AllSkillComponentList[i]->GetDefaultObject(), tmpGameState->AllSkillComponentList[i]->GetDefaultObject()->GetOuter())) );
		//	}

		//	// targeting comp
		//	SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(tmpGameState->MainSkillComponentList[2]->GetDefaultObject(), tmpGameState->MainSkillComponentList[2]->GetDefaultObject()->GetOuter())) );
		//	// execute comp
		//	SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(tmpGameState->MainSkillComponentList[1]->GetDefaultObject(), tmpGameState->MainSkillComponentList[1]->GetDefaultObject()->GetOuter())) );
		//}
	}
}

bool AComponentRPGCharacter::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (testRep != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(testRep, *Bunch, *RepFlags);
		WroteSomething |= testRep->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}

	if (SkillComponentDeck != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(SkillComponentDeck, *Bunch, *RepFlags);
		WroteSomething |= SkillComponentDeck->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}

	if (BuffDeck != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(BuffDeck, *Bunch, *RepFlags);
		WroteSomething |= BuffDeck->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}

	return WroteSomething;
}

void AComponentRPGCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AComponentRPGCharacter, testRep);
	DOREPLIFETIME(AComponentRPGCharacter, SkillComponentDeck);
	DOREPLIFETIME(AComponentRPGCharacter, BuffDeck);
	DOREPLIFETIME(AComponentRPGCharacter, TeamIndex);
	DOREPLIFETIME(AComponentRPGCharacter, IsCastingSkill);
	DOREPLIFETIME(AComponentRPGCharacter, EndLocation);
	DOREPLIFETIME(AComponentRPGCharacter, MoveToActor);
	DOREPLIFETIME(AComponentRPGCharacter, IsMovingToLocation);
	DOREPLIFETIME(AComponentRPGCharacter, IsMeleeAttacking);
	DOREPLIFETIME(AComponentRPGCharacter, IsPlayingCustomAnim);
	DOREPLIFETIME(AComponentRPGCharacter, myCustomTimeDilation);
	DOREPLIFETIME(AComponentRPGCharacter, bClientClearTargetingComponent);
	DOREPLIFETIME(AComponentRPGCharacter, ConsecutiveCastInterruptionCount);
	DOREPLIFETIME(AComponentRPGCharacter, bWaitingForAction);
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AComponentRPGCharacter::UpdateAnimation()
{
	if (IsPlayingCustomAnim)
		return;

	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
		GetSprite()->SetLooping(true);
		GetSprite()->PlayFromStart();
	}
}

void AComponentRPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FakeTickController();

	UpdateCharacter();	

	//UPawnMovementComponent* MovementComponent = GetMovementComponent();
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MovementComponent)
	{
		MovementComponent->CalcVelocity(DeltaSeconds, GetCharacterMovement()->GroundFriction, false, GetCharacterMovement()->BrakingDecelerationWalking);
	}

	///// calc stats
	if (CharStatComponent)
	{
		if (!IsCastingSkill)
		{
			//if (!IsMovingToLocation)
				CharStatComponent->CurrentStats.BASE_ACTIONBAR += CharStatComponent->BaseStats.BASE_SPEED * DeltaSeconds;

			if (CharStatComponent->CurrentStats.BASE_ACTIONBAR >= CharStatComponent->BaseStats.BASE_ACTIONBAR)
			{
				// cappped
				CharStatComponent->CurrentStats.BASE_ACTIONBAR = CharStatComponent->BaseStats.BASE_ACTIONBAR;

				if (Role == ROLE_Authority)
				{
					if (!bWaitingForAction)
					{
						Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->WaitForCharacterTurn(this);
					}
				}
			}
		}
		else
		{
			// casting skill
			CharStatComponent->CurrentStats.BASE_CASTBAR += CharStatComponent->BaseCastingSpeed * SkillComponentDeck->GetTotalCastRateModifier() * DeltaSeconds;

			if (CharStatComponent->CurrentStats.BASE_CASTBAR >= CharStatComponent->BaseStats.BASE_CASTBAR)
			{
				// capped
				CharStatComponent->CurrentStats.BASE_CASTBAR = CharStatComponent->BaseStats.BASE_CASTBAR;

				if (Role == ROLE_Authority)
				{
					// cast skill
					SkillComponentDeck->FinishCastingSkill();

					IsCastingSkill = false;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AComponentRPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	//// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	//InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//
	//InputComponent->BindAxis("MoveRight", this, &AComponentRPGCharacter::MoveRight);

	//InputComponent->BindTouch(IE_Pressed, this, &AComponentRPGCharacter::TouchStarted);
	//InputComponent->BindTouch(IE_Released, this, &AComponentRPGCharacter::TouchStopped);

	//InputComponent->BindAction("HttpRequest", IE_Released, this, &AComponentRPGCharacter::HttpRequest);
}

void AComponentRPGCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AComponentRPGCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AComponentRPGCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AComponentRPGCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (TravelDirection < 0.0f)
	{
		SetActorRotation(FRotator(0.0, 180.0f, 0.0f));
	}
	else if (TravelDirection > 0.0f)
	{
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

void AComponentRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCharacter();
}

void AComponentRPGCharacter::OnHitEvent(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role != ROLE_Authority)
		return;

	if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) )
	{
		if (IsMeleeAttacking)
		{
			if (OtherActor->IsA(AComponentRPGCharacter::StaticClass()))
			{
				// dont melee attack same team
				if (Cast<AComponentRPGCharacter>(OtherActor)->TeamIndex == TeamIndex)
					return;

				// calc skill deck effect here
				AComponentRPG_BuffActor* tmpBuff;
				tmpBuff = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->SpawnBuffActor();
				if (tmpBuff)
				{
					tmpBuff->m_BuffType= BuffType::BuffType_Damage;
					tmpBuff->BuffDamage = CharStatComponent->BaseDamage;
					tmpBuff->bIsInstantBuff = true;

					// modify buff by each SKillComp
					for (int i = 0; i < SkillComponentDeck->SkillComponentArray.Num(); ++i)
					{
						SkillComponentDeck->SkillComponentArray[i]->BuffActorModification(tmpBuff);
					}

					tmpBuff->Instigator = this;
					Cast<AComponentRPGCharacter>(OtherActor)->BuffDeck->AddBuffToDeck(tmpBuff);
				}

				PlayCustomAnim(AttackAnimation);

				SetIsMeleeAttacking(false);
				IsMovingToLocation = false;
				MoveToActor = nullptr;
			}
		}
	}
}

void AComponentRPGCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AComponentRPGCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

bool AComponentRPGCharacter::DestroyCharacter()
{
	//// kill CharacterHUD UI
	//// on server only
	//CurrentWidget->RemoveFromParent();
	//CurrentWidget->RemoveFromViewport();

	if (Role != ROLE_Authority)
		return false;
	
	if (IsPendingKill())
		return false;

	// kill all buffs
	for (int i = 0; i < BuffDeck->BuffArray.Num(); ++i)
	{
		if (BuffDeck->BuffArray[i])
			BuffDeck->BuffArray[i]->DestroyBuffActor();
	}

	// remove character from array
	Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->CharacterArray.Remove(this);

	return Destroy();
}

void AComponentRPGCharacter::SetupCharacter()
{
	//if (Role == ROLE_Authority)
	//{
	//	CharStatComponent->ComponentInit();
	//}

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPGCharacter::ClientCreateCharacterWidget, 0.5f, false);
}

void AComponentRPGCharacter::ClientCreateCharacterWidget_Implementation()
{
	CheckCurrentWidgetTimer();
}

void AComponentRPGCharacter::CheckCurrentWidgetTimer()
{
	CreateCharacterWidget();

	if (CurrentWidget == nullptr)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPGCharacter::CheckCurrentWidgetTimer, 0.2f, false);
	}
}

void AComponentRPGCharacter::CreateCharacterWidget()
{
	AComponentRPG_GameState* myGameState = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (myGameState == nullptr || GetWorld() == nullptr)
	{
		return;
	}

	///// setup HUD
	TSubclassOf<UComponentRPG_UserWidget> StartingWidgetClass = myGameState->PlayerHUD_UI;
	if (StartingWidgetClass != nullptr)
	{
		UComponentRPG_UserWidget* tmpWidget = CreateWidget<UComponentRPG_UserWidget>(GetWorld(), StartingWidgetClass);
		if (tmpWidget != nullptr)
		{
			tmpWidget->SetWidgetOwner(this);
			tmpWidget->AddToViewport();

			CurrentWidget = tmpWidget;
		}
	}
}

void AComponentRPGCharacter::CharTakeDamage(AComponentRPG_BuffActor* DamageCauser, float damage_amount)
{
	if (Role != ROLE_Authority)
		return;

	float final_damage = damage_amount;
	///// reduce damage taken with DEF stat
	// TODO

	///// taking damage
	CharStatComponent->CurrentStats.BASE_HP -= final_damage;

	///// interrupt casting
	if (IsCastingSkill)
	{
		if (final_damage > CharStatComponent->BaseStats.BASE_HP * 0.1f)
		{
			// take too much damage - interrupt casting
			IsCastingSkill = false;
			CharStatComponent->CurrentStats.BASE_CASTBAR = 0;
			ConsecutiveCastInterruptionCount++;

			float tmpInterruptModifier = 0.25f * ConsecutiveCastInterruptionCount;
			if (tmpInterruptModifier > 1.0f)
				tmpInterruptModifier = 1.0f;

			CharStatComponent->CurrentStats.BASE_ACTIONBAR = CharStatComponent->CurrentStats.BASE_ACTIONBAR * tmpInterruptModifier;
		}
	}

	if (CharStatComponent->CurrentStats.BASE_HP <= 0)
	{
		DestroyCharacter();
	}
}

void AComponentRPGCharacter::StartCastingSkill()
{
	IsCastingSkill = true;
}

///// Animation Related)
void AComponentRPGCharacter::PlayCustomAnim(class UPaperFlipbook* anim)
{
	IsPlayingCustomAnim = true;
	GetSprite()->SetFlipbook(anim);
	GetSprite()->SetLooping(false);
	GetSprite()->PlayFromStart();
}
	
void AComponentRPGCharacter::OnCustomAnimFinish()
{
	if (GetSprite()->GetFlipbook() == AttackAnimation)
	{
		// finish attacking - move back to your territory
		SetMoveToLocation( Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->GetSpawnPosition(TeamIndex) );
	}

	IsPlayingCustomAnim = false;
}
///// end of Animation Related

///// move to function
void AComponentRPGCharacter::FakeTickController()
{
	FVector currentPosition = GetActorLocation();
	FVector MoveDirection;

	if (MoveToActor != nullptr)
	{
		// move to actor
		MoveDirection = (MoveToActor->GetActorLocation() - currentPosition);
	}
	else
	{
		// move to location
		MoveDirection = (EndLocation - currentPosition);
	}

	if (MoveDirection.Size() < EndLocationRadiusCheck)
	{
		SetIsMeleeAttacking(false);
		IsMovingToLocation = false;
		MoveToActor = nullptr;
		
		GetMovementComponent()->StopMovementImmediately();

		/////
		SkillComponentDeck->GetMoveCommandComponent()->ClearAllTarget();
	}

	if (IsMovingToLocation)
	{
		MoveDirection.Normalize();
		AddMovementInput(MoveDirection, 1);
	}
}

void AComponentRPGCharacter::SetMoveToLocation(FVector location)
{
	EndLocation = location;
	IsMovingToLocation = true;
}

void AComponentRPGCharacter::SetMoveToActor(AActor* targetActor)
{
	MoveToActor = targetActor;
	IsMovingToLocation = true;
}

void AComponentRPGCharacter::SetIsMeleeAttacking(bool val)
{
	IsMeleeAttacking = val;
}

void AComponentRPGCharacter::SetCustomTimeDilation(float val)
{
	myCustomTimeDilation = val;
	CustomTimeDilation = val;
}

void AComponentRPGCharacter::OnRep_myCustomTimeDilation()
{
	CustomTimeDilation = myCustomTimeDilation;
}
///// end of move to function

///// clear targeting component hack
void AComponentRPGCharacter::OnRep_ClientClearTargetingComponent()
{
	if (bClientClearTargetingComponent)
	{
		SkillComponentDeck->GetTargetingComponent()->ClearAllTarget();
		SkillComponentDeck->GetTargetingComponent()->ServerClearAllTarget();
	}
}

void AComponentRPGCharacter::SetClientClearTargetingComponent(bool val)
{
	bClientClearTargetingComponent = val;

	SkillComponentDeck->GetTargetingComponent()->ClearAllTarget();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPGCharacter::ResetClientClearTargetingComponent, 1.0f, false);
}

void AComponentRPGCharacter::ResetClientClearTargetingComponent()
{
	bClientClearTargetingComponent = false;
}
///// end of clear targeting component hack

/////// clear movement component hack
//void AComponentRPGCharacter::OnRep_ClientClearMovementComponent()
//{
//	if (bClientClearMovementComponent)
//	{
//		SkillComponentDeck->GetMoveCommandComponent()->ClearAllTarget();
//		SkillComponentDeck->GetMoveCommandComponent()->ServerClearAllTarget();
//	}
//}
//
//void AComponentRPGCharacter::SetClientClearMovementComponent(bool val)
//{
//	bClientClearMovementComponent = val;
//
//	SkillComponentDeck->GetMoveCommandComponent()->ClearAllTarget();
//
//	FTimerHandle handle;
//	GetWorld()->GetTimerManager().SetTimer(handle, this, &AComponentRPGCharacter::ResetClientClearMovementComponent, 1.0f, false);
//}
//
//void AComponentRPGCharacter::ResetClientClearMovementComponent()
//{
//	bClientClearMovementComponent = false;
//}
/////// end of clear movement component hack


//void AComponentRPGCharacter::HttpRequest()
//{
//	/*TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//
//	JsonObject->SetStringField(TEXT("some_string_field"), *FString::Printf(TEXT("%s"), *SomeFStringVariable));
//
//	FString OutputString;
//
//	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
//
//	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);*/
//
//	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
//
//	HttpRequest->SetVerb("POST");
//
//	HttpRequest->SetHeader("Content-Type", "application/json");
//
//	////HttpRequest->SetURL(*FString::Printf(TEXT("%s"), *UrlAddressAsString));
//	HttpRequest->SetURL("http://componentgames.pkunarochrakse.com/ComponentRPG/getuser.php");
//
//	HttpRequest->SetContentAsString("post data");
//
//	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AComponentRPGCharacter::HttpResponse);
//
//	HttpRequest->ProcessRequest();
//}
//
//void AComponentRPGCharacter::HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
//	if (bWasSuccessful)// && Response->GetContentType() == "application/json")
//	{
//		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//
//		FString tmp = Response->GetContentAsString();
//		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(tmp);
//
//		FJsonSerializer::Deserialize(JsonReader, JsonObject);
//
//		UI_Response = "";
//
//		TArray<TSharedPtr<FJsonValue>> tmp1 = JsonObject->GetArrayField("response");
//		UI_Response += "{";
//		UI_Response += tmp1[0]->AsObject()->GetStringField("email");
//		UI_Response += ",";
//		UI_Response += tmp1[0]->AsObject()->GetStringField("username");
//		UI_Response += ",";
//		UI_Response += tmp1[0]->AsObject()->GetStringField("password");
//		UI_Response += "},\n";
//
//		UI_Response += "{";
//		UI_Response += tmp1[1]->AsObject()->GetStringField("email");
//		UI_Response += ",";
//		UI_Response += tmp1[1]->AsObject()->GetStringField("username");
//		UI_Response += ",";
//		UI_Response += tmp1[1]->AsObject()->GetStringField("password");
//		UI_Response += "}";
//
//		CharStatComponent->ComponentInit();
//	}
//	else
//	{
//		// Handle error here
//	}
//}