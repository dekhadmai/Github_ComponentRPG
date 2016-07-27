// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_PlayerController.h"
#include "UI_RPG_Login.h"


FText UUI_RPG_Login::GetText_LoginStatus()
{
	FString result = "Not Login";
	AComponentRPG_PlayerController* tmpController = Cast<AComponentRPG_PlayerController>(myController);
	if (tmpController != nullptr)
	{
		if (tmpController->mySaveProfile != nullptr)
		{
			if (tmpController->mySaveProfile->CharProfileArray.Num() >= 3)
				result = "Save file ready";
			else
				result = "Fail to login, try again";
		}
	}

	return FText::FromString(result);
}

FText UUI_RPG_Login::GetText_Host_IP()
{
	FString result = "";
	//result += "HostFail " + FString::FromInt(HostFailTime);

	return FText::FromString(result);
}

void UUI_RPG_Login::OnClicked_Login(FString username)
{
	AComponentRPG_PlayerController* tmpController = Cast<AComponentRPG_PlayerController>(myController);
	if (tmpController != nullptr)
	{
		tmpController->Server_Http_LoadProfile(username);
	}
}

void UUI_RPG_Login::OnClicked_Join(FString DestIp)
{
	AComponentRPG_PlayerController* tmpController = Cast<AComponentRPG_PlayerController>(myController);
	if (tmpController != nullptr)
	{
		tmpController->ClientTravel( DestIp, TRAVEL_Absolute );
	}
}

