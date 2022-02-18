// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDRinternGameMode.h"
#include "GDRinternPlayerController.h"
#include "GDRinternPawn.h"

AGDRinternGameMode::AGDRinternGameMode()
{
	// no pawn by default
	DefaultPawnClass = AGDRinternPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AGDRinternPlayerController::StaticClass();
}
