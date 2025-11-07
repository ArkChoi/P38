// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyPlayerController.h"


//CDO 초기화 할때 용도, class 구조 잡거나, 초기화 할때
AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPawn::StaticClass(); //ClassName
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
