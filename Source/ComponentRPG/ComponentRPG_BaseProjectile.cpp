// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_BaseProjectile.h"

AComponentRPG_BaseProjectile::AComponentRPG_BaseProjectile()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> ProjectileAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> ProjectileAOEAnimationAsset;
		FConstructorStatics()
			: ProjectileAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/ProjectileAnim.ProjectileAnim")),
			ProjectileAOEAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/ProjectileAOEAnim.ProjectileAOEAnim"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	bAlwaysRelevant = true;

	ProjectileAnimation = ConstructorStatics.ProjectileAnimationAsset.Get();
	ProjectileAOEAnimation = ConstructorStatics.ProjectileAOEAnimationAsset.Get();
	GetSprite()->SetFlipbook(ProjectileAnimation);

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleRadius(15.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(15.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AComponentRPG_BaseProjectile::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AComponentRPG_BaseProjectile::OnEndOverlap);

	// Configure character movement
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//myInitialSpeed = 1200.0f;
	myInitialSpeed = 600.0f;
	ProjectileMovement->InitialSpeed = myInitialSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;


	/////
	CharStatComponent = CreateDefaultSubobject<UComponentRPG_CharStatComponent>(TEXT("CharStatComponent"));
	CharStatComponent->SetNetAddressable();
	CharStatComponent->SetIsReplicated(true);

	bUseLifeSpan = false;
	bSkipOverlapCheck = false;
	bDestroySelfWhenReachDestination = false;

	SpawnAreaBuffWhenDestroyed = false;
	HomingMagnitude = 30.0f;
}

bool AComponentRPG_BaseProjectile::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (SkillComponentDeck != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(SkillComponentDeck, *Bunch, *RepFlags);
		WroteSomething |= SkillComponentDeck->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}

	return WroteSomething;
}

void AComponentRPG_BaseProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, SkillComponentDeck);
	//DOREPLIFETIME(AComponentRPG_BaseProjectile, CharStatComponent);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, TeamIndex);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, DestPosition);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, DestActor);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, ProjOwner);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, bUseLifeSpan);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, myInitialSpeed);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, LifeSpan);
	DOREPLIFETIME(AComponentRPG_BaseProjectile, myCustomTimeDilation);
}

void AComponentRPG_BaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SkillComponentDeck = NewObject<UComponentRPG_SkillComponentDeck>(this);
		SkillComponentDeck->SetDeckOwner(this);
		SkillComponentDeck->Init();
	}
}

void AComponentRPG_BaseProjectile::UpdateCharacter()
{
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

void AComponentRPG_BaseProjectile::SetProjectileAnim(int animIdx)
{
	switch (animIdx)
	{
	case 0 :
		GetSprite()->SetFlipbook(ProjectileAnimation);
		break;
	case 1 :
		GetSprite()->SetFlipbook(ProjectileAOEAnimation);
		break;
	default:
		break;
	}
}

void AComponentRPG_BaseProjectile::SetTargetingTypeAOE()
{
	SetProjectileAnim(1);
	bSkipOverlapCheck = true;
	bDestroySelfWhenReachDestination = true;
	SpawnAreaBuffWhenDestroyed = true;
}

void AComponentRPG_BaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role != ROLE_Authority)
		return;

	if (bSkipOverlapCheck)
		return;

	if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) && (OtherActor != Instigator) )
	{
		if (OtherActor->IsA(AComponentRPGCharacter::StaticClass()))
		{
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

				tmpBuff->Instigator = Instigator;
				Cast<AComponentRPGCharacter>(OtherActor)->BuffDeck->AddBuffToDeck(tmpBuff);
			}

			// destroy the projectile
			DestroyProjectile();
		}
	}
}

void AComponentRPG_BaseProjectile::OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

bool AComponentRPG_BaseProjectile::DestroyProjectile()
{
	if (Role != ROLE_Authority)
		return false;

	if (IsPendingKill())
		return false;

	if (SpawnAreaBuffWhenDestroyed)
	{
		AComponentRPG_BaseProjectile* tmpProjectile;
		FVector SpawnLoc = GetActorLocation();
		SpawnLoc.Y -= 1.0f;
		tmpProjectile = Cast<AComponentRPG_GameState>(UGameplayStatics::GetGameState(GetWorld()))->SpawnProjectileActor(SpawnLoc, GetActorRotation(), NULL, true);
		if (tmpProjectile)
		{
			ProjectileCommonSetup(tmpProjectile, Cast<AComponentRPGCharacter>(Instigator));
		}
	}

	return Destroy();
}

void AComponentRPG_BaseProjectile::ProjectileCommonSetup(AComponentRPG_BaseProjectile* tmp_proj, AComponentRPGCharacter* tmp_char)
{
	///// common projectile setup
	tmp_proj->TeamIndex = tmp_char->TeamIndex;
	tmp_proj->ProjOwner = tmp_char;
	tmp_proj->Instigator = tmp_char;

	tmp_proj->SetActorLifeSpan(0.5f);

	bool isOnGroundBuff = false;
	for (int i = 0; i < SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray.Num(); ++i)
	{
		if (Cast<URPG_SkillComponent_ChangeToBuff>(SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray[i])->bIsGroundBuff)
		{
			tmp_proj->SetActorLifeSpan(Cast<URPG_SkillComponent_ChangeToBuff>(SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray[i])->BuffDuration);
			break;
		}
	}

	tmp_proj->SetActorSizeMultiplier( Cast<URPG_SkillComponent_Targeting>(SkillComponentDeck->GetTargetingComponent())->GetAreaSize() );

	// add skill comp from deck to projectile here
	for (int i = 0; i < SkillComponentDeck->SkillComponentArray.Num(); ++i)
	{
		if (SkillComponentDeck->SkillComponentArray[i]->GetIsSelected())
		{
			tmp_proj->AddDuplicateSkillComponentToDeck(SkillComponentDeck->SkillComponentArray[i]);
		}
	}

	// add stat
	tmp_proj->CharStatComponent = DuplicateObject(CharStatComponent, GetOuter());

	tmp_proj->GetCapsuleComponent()->bGenerateOverlapEvents = true;
}

void AComponentRPG_BaseProjectile::AddDuplicateSkillComponentToDeck(UComponentRPG_SkillComponent* skill_comp)
{
	SkillComponentDeck->AddSkillComponentToDeck( DuplicateObject(skill_comp, skill_comp->GetOuter()) );
}

void AComponentRPG_BaseProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bUseLifeSpan)
	{
		LifeSpan -= DeltaSeconds;

		if (LifeSpan <= 0.0f)
			DestroyProjectile();
	}

	if (DestActor != nullptr)
	{
		FVector currentPosition = GetActorLocation();
		FVector MoveDirection = (DestActor->GetActorLocation() - currentPosition);

		MoveDirection.Normalize();
		ProjectileMovement->Velocity = MoveDirection * ProjectileMovement->InitialSpeed;
	}

	if (bDestroySelfWhenReachDestination)
	{
		FVector currentPosition = GetActorLocation();
		FVector MoveDirection = (DestPosition - currentPosition);

		if ( FVector::DotProduct(MoveDirection, ProjectileMovement->Velocity) < 0 )
		{
			DestroyProjectile();
		}
	}
}


void AComponentRPG_BaseProjectile::SetInitialSpeed(float val)
{
	myInitialSpeed = val;
	ProjectileMovement->InitialSpeed = val;
}

void AComponentRPG_BaseProjectile::OnRep_myInitialSpeed()
{
	ProjectileMovement->InitialSpeed = myInitialSpeed;
}

void AComponentRPG_BaseProjectile::SetActorLifeSpan( float InLifespan )
{
	bUseLifeSpan = true;
	LifeSpan = InLifespan;
}

void AComponentRPG_BaseProjectile::SetActorSizeMultiplier(float size_mult)
{
	SetActorScale3D(FVector(size_mult,size_mult,size_mult));
}

float AComponentRPG_BaseProjectile::GetLifeSpan() const
{
	return LifeSpan;
}

void AComponentRPG_BaseProjectile::SetCustomTimeDilation(float val)
{
	myCustomTimeDilation = val;
	CustomTimeDilation = val;
}

void AComponentRPG_BaseProjectile::OnRep_myCustomTimeDilation()
{
	CustomTimeDilation = myCustomTimeDilation;
}