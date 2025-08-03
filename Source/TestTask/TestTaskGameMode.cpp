// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskGameMode.h"
#include "TestTaskCharacter.h"
#include "HUDBase.h"
#include "GS_GameStateBase.h"
#include "UObject/ConstructorHelpers.h"

ATestTaskGameMode::ATestTaskGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		GameStateClass = AGS_GameStateBase::StaticClass();
		HUDClass = AHUDBase::StaticClass();
	}
}
