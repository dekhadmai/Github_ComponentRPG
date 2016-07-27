// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_BuffActor.h"


// Sets default values
AComponentRPG_BuffActor::AComponentRPG_BuffActor()
{
	// Try to create the sprite component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		//Sprite->AttachParent = GetCapsuleComponent();
		//Sprite->AttachParent = GetRootComponent();
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->bGenerateOverlapEvents = false;
	}

	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BuffAnimationAsset;
		FConstructorStatics()
			: BuffAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/BuffAuraAnim.BuffAuraAnim"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	BuffAnimation = ConstructorStatics.BuffAnimationAsset.Get();
	Sprite->SetFlipbook(BuffAnimation);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsInstantBuff = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	bUseLifeSpan = false;
}

void AComponentRPG_BuffActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AComponentRPG_BuffActor, bIsInstantBuff);
	DOREPLIFETIME(AComponentRPG_BuffActor, m_BuffType);
	DOREPLIFETIME(AComponentRPG_BuffActor, BuffDamage);
	DOREPLIFETIME(AComponentRPG_BuffActor, BuffDamageType);
	DOREPLIFETIME(AComponentRPG_BuffActor, bUseLifeSpan);
	DOREPLIFETIME(AComponentRPG_BuffActor, OriginalLifeSpan);
	DOREPLIFETIME(AComponentRPG_BuffActor, LifeSpan);
	DOREPLIFETIME(AComponentRPG_BuffActor, myCustomTimeDilation);
}

void AComponentRPG_BuffActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorScale3D(FVector(1.5f, 1.5f, 1.0f));
}

// Called when the game starts or when spawned
void AComponentRPG_BuffActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AComponentRPG_BuffActor::StartTick()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AComponentRPG_BuffActor::DestroyBuffActor()
{
	if (Role != ROLE_Authority)
		return false;

	if (IsPendingKill())
		return false;

	return Destroy();
}

// Called every frame
void AComponentRPG_BuffActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	//if (Role != ROLE_Authority)
	//	return;

	if (GetOwner() == nullptr)
		return;

	if (bUseLifeSpan)
	{
		LifeSpan -= DeltaTime;

		if (LifeSpan <= 0.0f)
			DestroyBuffActor();
	}

	if (bIsInstantBuff)
	{
		// do something here
		if (BuffDamage != 0.0f)
		{
			if (Cast<AComponentRPGCharacter>(GetOwner()) != nullptr)
			{
				Cast<AComponentRPGCharacter>(GetOwner())->CharTakeDamage(this, BuffDamage);
			}
		}

		DestroyBuffActor();
	}
	else
	{
		// do something here
		if (BuffDamage != 0.0f)
		{
			if (Cast<AComponentRPGCharacter>(GetOwner()) != nullptr)
			{
				FVector tmpLoc = Cast<AComponentRPGCharacter>(GetOwner())->GetActorLocation();
				tmpLoc.Y += 2.0f;
				SetActorLocation(tmpLoc);
				Cast<AComponentRPGCharacter>(GetOwner())->CharTakeDamage(this, BuffDamage * 2.0f * DeltaTime / OriginalLifeSpan);
			}
		}
	}
}

BuffType AComponentRPG_BuffActor::GetBuffType()
{
	return m_BuffType;
}

FString AComponentRPG_BuffActor::GetBuffDescription()
{
	FString result = "";
	if (bIsInstantBuff)
		result += "Instant : ";
	else
	{
		result += "Buff : ";
		result += "(";
		result += FString::Printf(TEXT("%.2f"), GetLifeSpan());
		result += ")";
		result += " : ";
	}

	FString strType = "";
	switch (BuffDamageType)
	{
		case DamageType::DamageType_NoElement : 
			strType = "NonElement"; break;
		case DamageType::DamageType_Fire : 
			strType = "Fire"; break;
		case DamageType::DamageType_Ice : 
			strType = "Ice"; break;
		case DamageType::DamageType_Lightning : 
			strType = "Lightning"; break;
		case DamageType::DamageType_Poison : 
			strType = "Poison"; break;
		case DamageType::DamageType_Heal : 
			strType = "Heal"; break;
		default : break;
	}

	result += FString::SanitizeFloat(BuffDamage);
	result += " " + strType;
	result += " Damage";

	return result;
}


void AComponentRPG_BuffActor::SetActorLifeSpan( float InLifespan )
{
	bUseLifeSpan = true;
	OriginalLifeSpan = InLifespan;
	LifeSpan = InLifespan;
}

float AComponentRPG_BuffActor::GetLifeSpan() const
{
	return LifeSpan;
}

void AComponentRPG_BuffActor::SetCustomTimeDilation(float val)
{
	myCustomTimeDilation = val;
	CustomTimeDilation = val;
}

void AComponentRPG_BuffActor::OnRep_myCustomTimeDilation()
{
	CustomTimeDilation = myCustomTimeDilation;
}