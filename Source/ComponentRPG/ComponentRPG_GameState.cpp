// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentRPG.h"
#include "ComponentRPG_GameState.h"
#include "ComponentRPG_PlayerController.h"
#include "PaperTileMapActor.h"

AComponentRPG_GameState::AComponentRPG_GameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameManager = CreateDefaultSubobject<UComponentRPG_GameManager>(TEXT("GameManager"));

	TileSize = 32.0f * 4.0f;
	SetupCount = 0;

	//MainSkillComponentList.Add(URPG_SkillComponent_MoveCommand::StaticClass());
	MainSkillComponentList.Add(URPG_SkillComponent_MoveOrder::StaticClass());
	MainSkillComponentList.Add(URPG_SkillComponent_Execute::StaticClass());
	MainSkillComponentList.Add(URPG_SkillComponent_Targeting::StaticClass());

	AllSkillComponentList.Add(URPG_SkillComponent_Damage::StaticClass());			// 0
	AllSkillComponentList.Add(URPG_SkillComponent_DamageType::StaticClass());		// 1
	AllSkillComponentList.Add(URPG_SkillComponent_PlusNumTarget::StaticClass());	// 2
	AllSkillComponentList.Add(URPG_SkillComponent_PlusAreaSize::StaticClass());		// 3
	AllSkillComponentList.Add(URPG_SkillComponent_ChangeToBuff::StaticClass());		// 4
	AllSkillComponentList.Add(URPG_SkillComponent_ChanceOnHit::StaticClass());		// 5

}

void AComponentRPG_GameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AComponentRPG_GameState, CurrentCharacterTurn);
	DOREPLIFETIME(AComponentRPG_GameState, CurrentCharacterStartTurnTime);
}

void AComponentRPG_GameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	///// search for all actors with tag name here
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		ACameraActor* tmpCamera = Cast<ACameraActor>(Actor);
		if (tmpCamera && tmpCamera->ActorHasTag("MainCamera"))
		{
			//SetViewTarget(mainCamera);
			MainCamera = tmpCamera;
		}

		APaperTileMapActor* tilemap = Cast<APaperTileMapActor>(Actor);
		if (tilemap && tilemap->ActorHasTag("Board"))
		{
			// get board position
			BoardPosition = tilemap->GetActorLocation();
			BoardPosition.X += TileSize / 4.0f;
			BoardPosition.Z -= TileSize / 4.0f;
			//myGameState->GameManager->BoardPosition.Y = -13.0f;

			BoardRotation = tilemap->GetActorRotation();
		}
	}
}

void AComponentRPG_GameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}


void AComponentRPG_GameState::GameStateInit(int player_index, UComponentRPG_SaveProfile* save_profile)
{
	///// init characters
	// spawn player 1 x=0-2, z=0-4

	if (Role == ROLE_Authority)
	{
		for (int32 i = 0; i < 3; ++i)
		{
			FVector SpawnLoc = GetSpawnPosition(player_index);

			AComponentRPGCharacter* character = GetWorld()->SpawnActor<AComponentRPGCharacter>(CharacterBlueprint, SpawnLoc, BoardRotation);
			character->TeamIndex = player_index;

			///// init StatPoint
			character->CharStatComponent->ComponentInit(save_profile->CharProfileArray[i].CharStatPoint, false);

			///// init SkillComp
			// move comp
			character->SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(MainSkillComponentList[0]->GetDefaultObject(), MainSkillComponentList[0]->GetDefaultObject()->GetOuter())) );

			// configurable comp
			for (int32 j = 0; j < 7; ++j)
			{
				UComponentRPG_SkillComponent* tmpSkillComp = Cast<UComponentRPG_SkillComponent>(DuplicateObject(AllSkillComponentList[save_profile->CharProfileArray[i].CharSkillComponentIndexArray[j].SkillComponentId]->GetDefaultObject(), AllSkillComponentList[save_profile->CharProfileArray[i].CharSkillComponentIndexArray[j].SkillComponentId]->GetDefaultObject()->GetOuter()));
				tmpSkillComp->ComponentSetup(save_profile->CharProfileArray[i].CharSkillComponentIndexArray[j]);
				character->SkillComponentDeck->AddSkillComponentToDeck( tmpSkillComp );
			}

			// targeting comp
			character->SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(MainSkillComponentList[2]->GetDefaultObject(), MainSkillComponentList[2]->GetDefaultObject()->GetOuter())) );
			// execute comp
			character->SkillComponentDeck->AddSkillComponentToDeck( Cast<UComponentRPG_SkillComponent>(DuplicateObject(MainSkillComponentList[1]->GetDefaultObject(), MainSkillComponentList[1]->GetDefaultObject()->GetOuter())) );
			///// end of init SkillComp

			CharacterArray.Add(character);
		}

		WaitForCharacterTurn();

		if (CharacterArray.Num() >= 6)
		{
			CharacterExecuteAction();
		}

		//SetupCount++;
	}
}

AComponentRPG_BaseProjectile* AComponentRPG_GameState::SpawnProjectileActor(FVector SpawnLoc, FRotator SpawnRot, APawn* proj_owner, bool isAreaBuff = false)
{
	FActorSpawnParameters params;
	params.Instigator = proj_owner;

	AComponentRPG_BaseProjectile* actor = nullptr;
	if (Role == ROLE_Authority)
	{
		TSubclassOf<class AComponentRPG_BaseProjectile> tmpTemplate;
		if (!isAreaBuff)
			tmpTemplate = ProjectileBlueprint;
		else
			tmpTemplate = AreaBuffBlueprint;
		
		actor = GetWorld()->SpawnActor<AComponentRPG_BaseProjectile>(tmpTemplate, SpawnLoc, SpawnRot, params);
		NonCharacterArray.Add(actor);
	}
	return actor;
}

AComponentRPG_BuffActor* AComponentRPG_GameState::SpawnBuffActor()
{
	AComponentRPG_BuffActor* actor = nullptr;
	if (Role == ROLE_Authority)
	{
		actor = GetWorld()->SpawnActor<AComponentRPG_BuffActor>();
		NonCharacterArray.Add(actor);
	}
	return actor;
}

void AComponentRPG_GameState::WaitForCharacterTurn(AComponentRPGCharacter* current_char)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle))
		return;

	if (current_char != nullptr)
	{
		CurrentCharacterTurn = current_char;

		CurrentCharacterStartTurnTime = GetWorld()->TimeSeconds;
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &AComponentRPG_GameState::EndTurnTimer, 10.0f, false);
		CurrentCharacterTurn->bWaitingForAction = true;
	}

	SetGlobalTimeDilation(0.0000001f);
}
void AComponentRPG_GameState::CharacterExecuteAction(AComponentRPGCharacter* current_char)
{
	SetGlobalTimeDilation(1.0f);
	GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	
	if (current_char != nullptr)
	{
		current_char->bWaitingForAction = false;
		CurrentCharacterTurn = nullptr;
	}
}

void AComponentRPG_GameState::EndTurnTimer()
{
	CurrentCharacterTurn->SkillComponentDeck->ExecuteAction();
}

void AComponentRPG_GameState::SetGlobalTimeDilation(float val)
{
	for (int i = 0; i < CharacterArray.Num(); ++i)
	{
		CharacterArray[i]->SetCustomTimeDilation(val);
	}

	for (int i = 0; i < NonCharacterArray.Num(); ++i)
	{
		AComponentRPG_BaseProjectile* tmpProj = Cast<AComponentRPG_BaseProjectile>(NonCharacterArray[i]);
		if (tmpProj)
		{
			tmpProj->SetCustomTimeDilation(val);
			continue;
		}

		AComponentRPG_BuffActor* tmpBuff = Cast<AComponentRPG_BuffActor>(NonCharacterArray[i]);
		if (tmpBuff)
		{
			tmpBuff->SetCustomTimeDilation(val);
			continue;
		}
	}
}

FVector AComponentRPG_GameState::GetTileCenterPosition(int32 x, int32 z)
{
	// 0,0 is the top left
	// 8,2 is the bottom right
	FVector result;

	result.X = BoardPosition.X + (TileSize * x);
	result.Z = BoardPosition.Z - (TileSize * z);
	result.Y = BoardPosition.Y + 1.0f;

	return result;
}

FVector AComponentRPG_GameState::GetSpawnPosition(int32 areaIndex)
{
	int32 randMinX = 0 + ((areaIndex == 0) ? 0 : 6);
	int32 randMaxX = 2 + ((areaIndex == 0) ? 0 : 6);
	int32 randMinY = 0;
	int32 randMaxY = 4;

	FVector SpawnLoc = GetTileCenterPosition(FMath::RandRange(randMinX, randMaxX), FMath::RandRange(randMinY, randMaxY));
	for (int32 i = 0; i < CharacterArray.Num(); ++i)
	{
		if (CharacterArray[i]->GetActorLocation() == SpawnLoc)
		{
			// same pos, repick the cell
			SpawnLoc = GetSpawnPosition(areaIndex);
			break;
		}
	}

	return SpawnLoc;
}

/////////////////////////////////////////////////////
////////// Http land, game code end here ///////////
///////////////////////////////////////////////////

void AComponentRPG_GameState::HttpRequest_LoadPlayerProfile(FString url, int controller_index)
{
	LastControllerRequest = controller_index;

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("Content-Type", "application/json");
	FString tmpURL = "http://componentgames.pkunarochrakse.com/ComponentRPG/player_profile_char.php?username=";
	tmpURL += url;

	HttpRequest->SetURL(tmpURL);
	HttpRequest->SetContentAsString("post data");
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AComponentRPG_GameState::HttpResponse_LoadPlayerProfile);
	HttpRequest->ProcessRequest();
}

void AComponentRPG_GameState::HttpResponse_LoadPlayerProfile(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)// && Response->GetContentType() == "application/json")
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FString tmp = Response->GetContentAsString();
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(tmp);
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

		AComponentRPG_PlayerController* myController = Cast<AComponentRPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), LastControllerRequest));
		UComponentRPG_SaveProfile* tmpSaveProfile = NewObject<UComponentRPG_SaveProfile>(myController);

		TArray<TSharedPtr<FJsonValue>> tmpResponse = JsonObject->GetArrayField("response");
		for (int i = 0; i < tmpResponse.Num(); ++i)
		{
			FCharacterProfile tmpCharProfile;

			tmpCharProfile.CharID = tmpResponse[i]->AsObject()->GetIntegerField("char_id");
			tmpCharProfile.CharIndex = tmpResponse[i]->AsObject()->GetIntegerField("char_index");
			tmpCharProfile.CharName = tmpResponse[i]->AsObject()->GetStringField("char_name");
			tmpCharProfile.CharLevel = tmpResponse[i]->AsObject()->GetIntegerField("char_level");
			
			tmpCharProfile.CharStatPoint.STATPOINT_DAMAGE = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_damage");
			tmpCharProfile.CharStatPoint.STATPOINT_EFFECTIVENESS = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_effectiveness");
			tmpCharProfile.CharStatPoint.STATPOINT_DEFENSE = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_defense");
			tmpCharProfile.CharStatPoint.STATPOINT_RESISTANCE = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_resistance");
			tmpCharProfile.CharStatPoint.STATPOINT_SPEED = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_speed");
			tmpCharProfile.CharStatPoint.STATPOINT_RECOVERY = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_recovery");
			tmpCharProfile.CharStatPoint.STATPOINT_MAXBAR = tmpResponse[i]->AsObject()->GetIntegerField("char_stat_maxbar");

			for (int j = 1; j <= 7; ++j)
			{
				FString ObjectFieldName = "char_skill_" + FString::FromInt(j);
				FSkillComponentCustomization tmpCompCustomize;
				TSharedPtr<FJsonObject> tmpSkillObject = tmpResponse[i]->AsObject()->GetObjectField(ObjectFieldName);
				tmpCompCustomize.SkillComponentId = tmpSkillObject->GetIntegerField("skillcomponent_id");
				tmpCompCustomize.Customize_int1 = tmpSkillObject->GetIntegerField("int1");
				tmpCompCustomize.Customize_int2 = tmpSkillObject->GetIntegerField("int2");
				tmpCompCustomize.Customize_int3 = tmpSkillObject->GetIntegerField("int3");
				tmpCompCustomize.Customize_int4 = tmpSkillObject->GetIntegerField("int4");
				tmpCompCustomize.Customize_float1 = tmpSkillObject->GetNumberField("float1");
				tmpCompCustomize.Customize_float2 = tmpSkillObject->GetNumberField("float2");
				tmpCompCustomize.Customize_float3 = tmpSkillObject->GetNumberField("float3");
				tmpCompCustomize.Customize_float4 = tmpSkillObject->GetNumberField("float4");
			
				tmpCharProfile.CharSkillComponentIndexArray.Add(tmpCompCustomize);
			}

			tmpSaveProfile->CharProfileArray.Add(tmpCharProfile);
		}
		
		myController->HttpResponse_LoadPlayerProfile(tmpSaveProfile);
	}
	else
	{
		// Handle error here
	}
}






/////// HttpRequest Template
//void AComponentRPG_GameState::HttpRequest()
//{
//	//TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//
//	//JsonObject->SetStringField(TEXT("some_string_field"), *FString::Printf(TEXT("%s"), *SomeFStringVariable));
//
//	//FString OutputString;
//
//	//TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
//
//	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
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
//	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AComponentRPG_GameState::HttpResponse);
//
//	HttpRequest->ProcessRequest();
//}
//
//void AComponentRPG_GameState::HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
//		FString UI_Response = "";
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
//		AComponentRPG_PlayerController* myController = Cast<AComponentRPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//		myController->HttpResponse_LoadPlayerProfile(UI_Response);
//	}
//	else
//	{
//		// Handle error here
//	}
//}