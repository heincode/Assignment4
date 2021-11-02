// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//on construction a percentage of tiles are set to be houses
	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	RootComponent = LocationComponent;
	if (FMath::FRandRange(1, 12) < 3)
	{
		House = true;
		CreateBuilding();
	}
	else
	{
		House = false;
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//This creates the building with a random level of floors and random rotation
void ATile::CreateBuilding()
{
	//initial floor is created
	UStaticMeshComponent* ProtoHouse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("protohouse"));
	ProtoHouse->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProtoHouseAsset(TEXT("/Game/Mesh/fixedbase.fixedbase"));

	if (ProtoHouseAsset.Succeeded())
	{


		//Rot value is randomised to be one of these rotations
		Rot = 0;    //results in base and roof
		RoofHeight = 385.0f;
		int32 Rand = FMath::FRandRange(1, 5);
		if (Rand == 2) //same
		{
			Rot = 90.0f;
			RoofHeight = 385.0f;
		}
		else if (Rand == 3) //base floor roof
		{
			Rot = 180.0f;
			RoofHeight = 783.0f;
		}
		else if (Rand == 4) //base floor floor roof
		{
			Rot = 270.0f;
			RoofHeight = 1182.0f;
		}

		//the initial floor is placed in the scene
		ProtoHouse->SetStaticMesh(ProtoHouseAsset.Object);
		ProtoHouse->SetRelativeRotation(FRotator(Rot, 0.0f, 0.0f));
		ProtoHouse->SetWorldScale3D(FVector(1.f));
		ProtoHouse->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));
	}

	if (Rot == 180.f || Rot == 270.0f)
	{
		//the next floor is created if needed
		UStaticMeshComponent* ProtoHouseLevel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("protohouselevel"));
		ProtoHouseLevel->SetupAttachment(RootComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ProtoHouseLevelAsset(TEXT("/Game/Mesh/fixedfloor.fixedfloor"));

		if (ProtoHouseLevelAsset.Succeeded())
		{
			ProtoHouseLevel->SetStaticMesh(ProtoHouseLevelAsset.Object);
			ProtoHouseLevel->SetRelativeRotation(FRotator(Rot, 0.0f, 0.0f));
			ProtoHouseLevel->SetWorldScale3D(FVector(1.f));
			ProtoHouseLevel->SetRelativeLocation(FVector(0.0f, -430.0f, 0.0f));
		}
		if (Rot == 270.0f)
		{
			//the next floor is created if needed
			UStaticMeshComponent* ProtoHouseLevel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("protohouselevel2"));
			ProtoHouseLevel2->SetupAttachment(RootComponent);

			static ConstructorHelpers::FObjectFinder<UStaticMesh> ProtoHouseLevel2Asset(TEXT("/Game/Mesh/fixedfloor.fixedfloor"));

			if (ProtoHouseLevelAsset.Succeeded())
			{
				ProtoHouseLevel2->SetStaticMesh(ProtoHouseLevel2Asset.Object);
				ProtoHouseLevel2->SetRelativeRotation(FRotator(Rot, 0.0f, 0.0f));
				ProtoHouseLevel2->SetWorldScale3D(FVector(1.f));
				ProtoHouseLevel2->SetRelativeLocation(FVector(0.0f, -830.0f, 0.0f));
			}
		}
	}

	//the roof is created and placed
	UStaticMeshComponent* Roof = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("roof"));
	Roof->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoofAsset(TEXT("/Game/Mesh/fixedroof.fixedroof"));

	if (RoofAsset.Succeeded())
	{
		Roof->SetStaticMesh(RoofAsset.Object);
		Roof->SetRelativeRotation(FRotator(Rot, 0.0f, 0.0f));
		Roof->SetWorldScale3D(FVector(1.f));
		Roof->SetRelativeLocation(FVector(0.0f, -RoofHeight, 0.0f));
	}


}

//trees are created and placed randomly within their tile at a random rotation
void ATile::GenerateTrees()
{
	if (!House && FMath::FRandRange(1, 3) < 2)
	{
		if (ToSpawn)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				FVector Location = this->GetActorLocation() + FVector(FMath::FRandRange(-500, 500), FMath::FRandRange(-500, 500), 150.0f);
				World->SpawnActor<AStaticMeshActor>(ToSpawn, Location, FRotator(0.0f, FMath::FRandRange(1, 180), 90.0f), spawnPar);
			}
		}
	}
}

bool ATile::ReturnHouse()
{
	return House;
}


