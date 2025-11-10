// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "Kismet/GameplayStatics.h"

#include "Roket.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//에디터 드래그 앤 드롭, 설정 다 코드로 변경 가능
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(46.0f, 62.0f, 10.0f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	//개발때 거의 안함..
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Meshs/SM_P38_Body.SM_P38_Body'")); // = class UStaticMesh SM_Body | 이거랑 같다고 한다.
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}


	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Left->SetRelativeLocation(FVector(37.f,-21.f,1.f));

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	Right->SetRelativeLocation(FVector(37.f, 21.f, 1.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/Meshs/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->SocketOffset = FVector(0, 0, 50.0f);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(70.f, 0, 0)); //RootComponent 에서 얼마만큼 떨어지는지를 표현한다고 한다.

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MoveMent"));
	Movement->MaxSpeed = 100.f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector());

	Left->AddLocalRotation(FRotator( 0, 0, 3600.f * DeltaTime));
	Right->AddLocalRotation(FRotator(0, 0, 3600.f * DeltaTime));

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Roll",this, &AMyPawn::Rotation_Roll);
	PlayerInputComponent->BindAxis("Pitch", this, &AMyPawn::Rotation_Pitch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyPawn::RoketFire);

}

void AMyPawn::Rotation_Roll(float AxisValue)
{
	this->AddActorLocalRotation(FRotator(0,0, AxisValue * 60.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}	

void AMyPawn::Rotation_Pitch(float AxisValue)
{
	this->AddActorLocalRotation(
		FRotator(
			AxisValue * 60.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 
			0, 
			0
		)
	);
}

void AMyPawn::RoketFire()
{
	this->GetWorld()->SpawnActor<ARoket>(ARoket::StaticClass(),Arrow->K2_GetComponentToWorld());
}