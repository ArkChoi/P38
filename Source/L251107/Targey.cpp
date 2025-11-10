// Fill out your copyright notice in the Description page of Project Settings.


#include "Targey.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATargey::ATargey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(32.f, 32.f, 32.f));

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Box);
	Sphere->SetRelativeScale3D(FVector(0.25f,1.f,1.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Sphere(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")); // = class UStaticMesh SM_Body | 이거랑 같다고 한다.
	if (SM_Sphere.Succeeded())
	{
		Sphere->SetStaticMesh(SM_Sphere.Object);
	}
}

// Called when the game starts or when spawned
void ATargey::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ATargey::ProcessActorAnyDamage);
}

// Called every frame
void ATargey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargey::ProcessActorAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Destroy();
}

