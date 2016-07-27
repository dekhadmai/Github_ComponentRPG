// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ComponentRPG_UserWidget.h"
#include "UI_RPG_Login.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRPG_API UUI_RPG_Login : public UComponentRPG_UserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetText_LoginStatus();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		FText GetText_Host_IP();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void OnClicked_Login(FString username);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void OnClicked_Join(FString DestIp);
};
