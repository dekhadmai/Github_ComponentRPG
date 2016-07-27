// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_CharStatComponent.h"


// Sets default values for this component's properties
UComponentRPG_CharStatComponent::UComponentRPG_CharStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	BaseCastingSpeed = 100.0f;
	BaseDamage = 30.0f;
}

void UComponentRPG_CharStatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UComponentRPG_CharStatComponent, StatPoints);
	DOREPLIFETIME(UComponentRPG_CharStatComponent, CurrentStatPoints);
	DOREPLIFETIME(UComponentRPG_CharStatComponent, BaseStats);
	DOREPLIFETIME(UComponentRPG_CharStatComponent, CurrentStats);

	//DOREPLIFETIME(UComponentRPG_CharStatComponent, Stats);
}


// Called when the game starts
void UComponentRPG_CharStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UComponentRPG_CharStatComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UComponentRPG_CharStatComponent::ComponentInit(FCharacterStatPoint profile_stat, bool bRandomStat)
{
	if (!bRandomStat)
	{
		StatPoints = profile_stat;
	}
	else
	{
		// grab char stat point
		StatPoints.STATPOINT_DAMAGE						= 10 * FMath::FRand();
		StatPoints.STATPOINT_EFFECTIVENESS				= 10 * FMath::FRand();
		StatPoints.STATPOINT_DEFENSE					= 10 * FMath::FRand();
		StatPoints.STATPOINT_RESISTANCE					= 10 * FMath::FRand();
		StatPoints.STATPOINT_SPEED						= 10 * FMath::FRand();
		StatPoints.STATPOINT_RECOVERY					= 10 * FMath::FRand();
		StatPoints.STATPOINT_MAXBAR						= 10 * FMath::FRand();
	}

	CurrentStatPoints = StatPoints;

	// grab char base stat (+equipment) and snapshot it to BaseStats
	BaseStats.BASE_HP						= 100 * (1 + StatPoints.STATPOINT_MAXBAR * 0.1f);
	BaseStats.BASE_MP						= 50  * (1 + StatPoints.STATPOINT_MAXBAR * 0.1f);
	BaseStats.BASE_ACTIONBAR				= 100;
	BaseStats.BASE_CASTBAR					= 100;
	RefreshBaseStats();

	// init current stats
	CurrentStats = BaseStats;
	CurrentStats.BASE_ACTIONBAR				= 0;
	CurrentStats.BASE_CASTBAR				= 0;
}

void UComponentRPG_CharStatComponent::RefreshBaseStats()
{
	///// put stat multipliers here (atk %, def diminishing return, etc.)
	BaseStats.BASE_DAMAGE					= 5 * CurrentStatPoints.STATPOINT_DAMAGE;
	BaseStats.BASE_EFFECTIVENESS			= 10 * CurrentStatPoints.STATPOINT_EFFECTIVENESS;
	BaseStats.BASE_DEFENSE					= 5 * CurrentStatPoints.STATPOINT_DEFENSE;
	BaseStats.BASE_RESISTANCE				= 10 * CurrentStatPoints.STATPOINT_RESISTANCE;
	BaseStats.BASE_SPEED					= 15 + (CurrentStatPoints.STATPOINT_SPEED * 1.0f);
	BaseStats.BASE_RECOVERY					= 5 * CurrentStatPoints.STATPOINT_RECOVERY;
}