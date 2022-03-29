// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileGDRGameMode.h"
#include "BallPawn.h"

AMobileGDRGameMode::AMobileGDRGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn>BPAWN(TEXT("Blueprint'/Game/BPawn.BPawn_C'"));
	DefaultPawnClass = BPAWN.Class;
}
