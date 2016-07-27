// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_TestRep.h"

UComponentRPG_TestRep::UComponentRPG_TestRep(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//tmp1 = CreateDefaultSubobject<URPG_SkillComponent_Targeting>(TEXT("Targeting"));
	//tmp2 = CreateDefaultSubobject<URPG_SkillComponent_Damage>(TEXT("Damage"));
	//tmp3 = CreateDefaultSubobject<URPG_SkillComponent_DamageType>(TEXT("DamageType"));
	//tmp4 = CreateDefaultSubobject<URPG_SkillComponent_PlusNumTarget>(TEXT("PlusNumTarget"));
	//tmp5 = CreateDefaultSubobject<URPG_SkillComponent_PlusNumTarget>(TEXT("PlusNumTarget2"));
	//tmp6 = CreateDefaultSubobject<URPG_SkillComponent_PlusAreaSize>(TEXT("PlusAreaSize"));
	//tmp7 = CreateDefaultSubobject<URPG_SkillComponent_PlusAreaSize>(TEXT("PlusAreaSize2"));
}

void UComponentRPG_TestRep::AddBuffActor(AComponentRPG_BuffActor* buff)
{
	testComp = buff;
}

void UComponentRPG_TestRep::Init()
{
	

	//tmp2->DamageAmount = 40;
	//tmp3->DmgType = DamageType::DamageType_Lightning;
	//tmp4->AdditionalNumTarget = 4;
	//tmp6->AdditionalAreaSize = 3;
	//tmp1->SetTargetSideType(TargetSideType::TargetSideType_Enemy);
	//tmp1->SetTargetingType(TargetingType::TargetingType_Melee);
	//testComp = tmp4;
	//testEnum = ChanceOnHitEffect::ChanceOnHitEffect_Stun;

	//testCompArray.Add(tmp1);
	//testCompArray.Add(tmp2);
	//testCompArray.Add(tmp3);
	//testCompArray.Add(tmp4);
	//testCompArray.Add(tmp5);
	//testCompArray.Add(tmp6);
	//testCompArray.Add(tmp7);
}

bool UComponentRPG_TestRep::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = false;

	//if (testComp != nullptr)
	//{
	//	WroteSomething |= Channel->ReplicateSubobject(testComp, *Bunch, *RepFlags);

	//	for (UComponentRPG_SkillComponent * myObject : testCompArray)
	//	{
	//		if (myObject != nullptr)
	//		{
	//			WroteSomething |= Channel->ReplicateSubobject(myObject, *Bunch, *RepFlags);
	//		}
	//	}
	//}

	return WroteSomething;
}

void UComponentRPG_TestRep::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UComponentRPG_TestRep, testComp);
	//DOREPLIFETIME(UComponentRPG_TestRep, testCompArray);
	//DOREPLIFETIME(UComponentRPG_TestRep, testEnum);
}

