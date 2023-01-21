// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoloprojectGameMode.h"
#include "SoloprojectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoloprojectGameMode::ASoloprojectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
