// Fill out your copyright notice in the Description page of Project Settings.


#include "Roket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Targey.h"


// Sets default values
ARoket::ARoket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//코드 작성
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(30.f, 10.f, 10.f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Box);
	StaticMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FQuat(FRotator(-90.0f, 0.0f, 0.0f)));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/Meshs/SM_Rocket.SM_Rocket'")); // = class UStaticMesh SM_Body | 이거랑 같다고 한다.
	if (SM_Rocket.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM_Rocket.Object);
	}


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 200.f;
	ProjectileMovement->MaxSpeed = 200.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ARoket::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.f);

	//Delegate UE
	OnActorBeginOverlap.AddDynamic(this,&ARoket::ProcessActorBeginOverlap);
}

// Called every frame
void ARoket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UGameplayStatics::ApplyDamage(
		OtherActor,
		10.f,
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		this,
		nullptr
	);

	if (dynamic_cast<ATargey*>(OtherActor))
	{
		Destroy();
	}
}


