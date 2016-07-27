// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_CharacterHUD.h"
#include "ComponentRPG_GameState.h"


void UComponentRPG_CharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UComponentRPG_CharacterHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetHUDPos();
}

void UComponentRPG_CharacterHUD::RefreshWidgetData()
{

}

void UComponentRPG_CharacterHUD::SetHUDPos()
{
	if (!myOwner)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (myOwner)
	{
		if (Cast<AComponentRPGCharacter>(myOwner)->IsPendingKill())
		{
			SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		FVector2D result;

		if (GetWorld()->GetFirstPlayerController() != NULL)
		{
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(Cast<AComponentRPGCharacter>(myOwner)->GetActorLocation(), result);
			SetPositionInViewport(result + FVector2D(-24.0f, -84.0f));
		}
	}
}

ESlateVisibility UComponentRPG_CharacterHUD::IsSelectedByController()
{
	if (!GetWorld()->GetFirstPlayerController() || !myOwner)
		return ESlateVisibility::Hidden;
	
	if (!Cast<AComponentRPG_PlayerController>(GetWorld()->GetFirstPlayerController())->CurrentWidget)
		return ESlateVisibility::Hidden;

	if (Cast<AComponentRPG_PlayerController>(GetWorld()->GetFirstPlayerController())->CurrentWidget->myOwner == myOwner)
		return ESlateVisibility::Visible;

	return ESlateVisibility::Hidden;
}

ESlateVisibility UComponentRPG_CharacterHUD::IsActionBarFull()
{
	if (GetPercentActionBar() >= 1.0f && !Cast<AComponentRPGCharacter>(myOwner)->IsCastingSkill && Cast<AComponentRPGCharacter>(myOwner)->bWaitingForAction)
	{
		return ESlateVisibility::Visible;
	}
	
	return ESlateVisibility::Hidden;
}

float UComponentRPG_CharacterHUD::GetPercentActionBar()
{
	if (myOwner)
	{
		///// update char stats
		return Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR / Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_ACTIONBAR;
	}
	else
	{
		return 0.0f;
	}
}

FText UComponentRPG_CharacterHUD::GetActionBarString()
{
	if (myOwner)
	{
		///// update char stats
		FString result = FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_ACTIONBAR) + "/" + FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_ACTIONBAR);
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}

float UComponentRPG_CharacterHUD::GetPercentCastBar()
{
	if (myOwner)
	{
		///// update char stats
		return Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_CASTBAR / Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_CASTBAR;
	}
	else
	{
		return 0.0f;
	}
}

FText UComponentRPG_CharacterHUD::GetCastBarString()
{
	if (myOwner)
	{
		///// update char stats
		FString result = FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_CASTBAR) + "/" + FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_CASTBAR);
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}

float UComponentRPG_CharacterHUD::GetPercentHP()
{
	if (myOwner)
	{
		///// update char stats
		return Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_HP / Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_HP;
	}
	else
	{
		return 0.0f;
	}
}

FText UComponentRPG_CharacterHUD::GetHPString()
{
	if (myOwner)
	{
		///// update char stats
		FString result = FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_HP) + "/" + FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_HP);
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}

float UComponentRPG_CharacterHUD::GetPercentMP()
{
	if (myOwner)
	{
		///// update char stats
		return Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_MP / Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_MP;
	}
	else
	{
		return 0.0f;
	}
}

FText UComponentRPG_CharacterHUD::GetMPString()
{
	if (myOwner)
	{
		///// update char stats
		FString result = FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->CurrentStats.BASE_MP) + "/" + FString::Printf(TEXT("%.0f"), Cast<AComponentRPGCharacter>(myOwner)->CharStatComponent->BaseStats.BASE_MP);
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}

FText UComponentRPG_CharacterHUD::GetStatPointString_Damage()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_DAMAGE);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_DAMAGE - tmpChar->CharStatComponent->StatPoints.STATPOINT_DAMAGE;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}
FText UComponentRPG_CharacterHUD::GetStatPointString_Effectiveness()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_EFFECTIVENESS);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_EFFECTIVENESS - tmpChar->CharStatComponent->StatPoints.STATPOINT_EFFECTIVENESS;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}
FText UComponentRPG_CharacterHUD::GetStatPointString_Defense()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_DEFENSE);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_DEFENSE - tmpChar->CharStatComponent->StatPoints.STATPOINT_DEFENSE;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}
FText UComponentRPG_CharacterHUD::GetStatPointString_Resistance()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_RESISTANCE);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_RESISTANCE - tmpChar->CharStatComponent->StatPoints.STATPOINT_RESISTANCE;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}
FText UComponentRPG_CharacterHUD::GetStatPointString_Speed()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_SPEED);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_SPEED - tmpChar->CharStatComponent->StatPoints.STATPOINT_SPEED;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}
FText UComponentRPG_CharacterHUD::GetStatPointString_Recovery()
{
	if (myOwner)
	{
		///// update char stats
		AComponentRPGCharacter* tmpChar = Cast<AComponentRPGCharacter>(myOwner);
		FString BaseStat = FString::Printf(TEXT("%.2f"), tmpChar->CharStatComponent->StatPoints.STATPOINT_RECOVERY);
		float ExtraStatFloat = tmpChar->CharStatComponent->CurrentStatPoints.STATPOINT_RECOVERY - tmpChar->CharStatComponent->StatPoints.STATPOINT_RECOVERY;

		FString ExtraStat = "(";
		if (ExtraStatFloat > 0)
			ExtraStat += "+";
		ExtraStat += FString::Printf(TEXT("%.2f"), ExtraStatFloat);
		ExtraStat += ")";
		FString result = BaseStat + ExtraStat;
			
		return  FText::FromString(result);
	}
	else
	{
		return FText::FromString("NAN");
	}
}