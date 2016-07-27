// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPGCharacter.h"
#include "ComponentRPG_GameState.h"
#include "RPG_SkillComponent_ChangeToBuff.h"
#include "ComponentRPG_AreaBuffActor.h"

void AComponentRPG_AreaBuffActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AComponentRPG_AreaBuffActor::OnBeginOverlap);

	//ProjectileMovement->bRotationFollowsVelocity = false;
	//ProjectileMovement->InitialSpeed = 0.0f;

	//if (HasAuthority())
	//{
	//	SkillComponentDeck = NewObject<UComponentRPG_SkillComponentDeck>(this);
	//	SkillComponentDeck->SetDeckOwner(this);
	//	SkillComponentDeck->Init();
	//}
}

void AComponentRPG_AreaBuffActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
				BuffApplyArray.Add(tmpBuff);
			}
		}
	}
}

void AComponentRPG_AreaBuffActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Role != ROLE_Authority)
		return;

	if (bSkipOverlapCheck)
		return;

	if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) && (OtherActor != Instigator) )
	{
		if (OtherActor->IsA(AComponentRPGCharacter::StaticClass()))
		{
			bool isOnGroundBuff = false;
			for (int i = 0; i < SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray.Num(); ++i)
			{
				if (Cast<URPG_SkillComponent_ChangeToBuff>(SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray[i])->bIsGroundBuff)
				{
					isOnGroundBuff = true;
					break;
				}
			}

			if (isOnGroundBuff)
			{
				for (int i = 0; i < BuffApplyArray.Num(); ++i)
				{
					if (Cast<AComponentRPGCharacter>(OtherActor)->BuffDeck->BuffArray.Find(BuffApplyArray[i]) != INDEX_NONE)
					{
						BuffApplyArray[i]->DestroyBuffActor();
						BuffApplyArray.RemoveAt(i);
						return;
					}
				}
			}
		}
	}
}

bool AComponentRPG_AreaBuffActor::DestroyProjectile()
{
	if (Role != ROLE_Authority)
		return false;

	bool isOnGroundBuff = false;
	for (int i = 0; i < SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray.Num(); ++i)
	{
		if (Cast<URPG_SkillComponent_ChangeToBuff>(SkillComponentDeck->SkillComponentArrayByType[(uint8)SkillComponentType::SkillComponentType_ChangeToBuff]->SkillComponentArray[i])->bIsGroundBuff)
		{
			isOnGroundBuff = true;
			break;
		}
	}

	if (isOnGroundBuff)
	{
		for (int i = 0; i < BuffApplyArray.Num(); ++i)
		{
			if (BuffApplyArray[i] != nullptr)
				BuffApplyArray[i]->DestroyBuffActor();
		}
	}

	return Super::DestroyProjectile();
}