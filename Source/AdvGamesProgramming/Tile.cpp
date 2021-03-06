// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Grid.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//on construction a percentage of tiles are set to be houses
	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	RootComponent = LocationComponent;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	if (TileNo != 0 && House == true && IsRoad == false)
	{
		CreateBuilding();
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//This creates the building with a random level of floors and random rotation
void ATile::CreateBuilding()
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

		//first level created
		if (ToSpawnHouse)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 30.0f);
				World->SpawnActor<AStaticMeshActor>(ToSpawnHouse, Location, FRotator(0.0f, Rot, 90.0f), spawnPar);
			}
		}
		// gun is possibly spawned on the first floor
		if (FMath::FRandRange(1, 4) > 3)
		{
			if (true)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters spawnPar;
					spawnPar.Owner = this;
					FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 40.0f);
					World->SpawnActor<AWeaponPickup>(ToSpawnGun, Location, FRotator(0.0f, 0.0f, 0.0f), spawnPar);
				}
			}
		}

		//second level created
		if (Rot == 180.f || Rot == 270.0f)
		{
			if (ToSpawnHouse2)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters spawnPar;
					spawnPar.Owner = this;
					FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 430.0f);
					World->SpawnActor<AStaticMeshActor>(ToSpawnHouse2, Location, FRotator(0.0f, Rot, 90.0f), spawnPar);
				}
			}
			// gun is possibly spawned on 2nd floor
			if (FMath::FRandRange(1, 5) > 3)
			{
				if (true)
				{
					UWorld* World = GetWorld();
					if (World)
					{
						FActorSpawnParameters spawnPar;
						spawnPar.Owner = this;
						FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 470.0f);
						World->SpawnActor<AWeaponPickup>(ToSpawnGun, Location, FRotator(0.0f, 0.0f, 0.0f), spawnPar);
					}
				}
			}
		}

		//third level created
		if (Rot == 270.0f)
		{
			if (ToSpawnHouse2)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters spawnPar;
					spawnPar.Owner = this;
					FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 830.0f);
					World->SpawnActor<AStaticMeshActor>(ToSpawnHouse2, Location, FRotator(0.0f, Rot, 90.0f), spawnPar);
				}
			}
			// gun is possibly spawned on the third floor
			if (FMath::FRandRange(1, 5) > 3)
			{
				if (true)
				{
					UWorld* World = GetWorld();
					if (World)
					{
						FActorSpawnParameters spawnPar;
						spawnPar.Owner = this;
						FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, 870.0f);
						World->SpawnActor<AWeaponPickup>(ToSpawnGun, Location, FRotator(0.0f, 0.0f, 0.0f), spawnPar);
					}
				}
			}
		}

		////the roof is created
		if (ToSpawnHouse3)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				FVector Location = this->GetActorLocation() + FVector(0.0f, 0.0f, RoofHeight);
				World->SpawnActor<AStaticMeshActor>(ToSpawnHouse3, Location, FRotator(0.0f, Rot, 90.0f), spawnPar);
			}
		}
}

//trees are created and placed randomly within their tile at a random rotation
void ATile::GenerateTrees()
{
	//chance to spawn trees
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
	//lower chance to spawn tree stumps
	if (!House && FMath::FRandRange(1, 9) > 7)
	{
		if (ToSpawnTreeStump)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				FVector Location = this->GetActorLocation() + FVector(FMath::FRandRange(-500, 500), FMath::FRandRange(-500, 500), 0.0f);
				World->SpawnActor<AStaticMeshActor>(ToSpawnTreeStump, Location, FRotator(0.0f, FMath::FRandRange(1, 180), 90.0f), spawnPar);
			}
		}
	}
}

//bushes are created and placed randomly within their tile at a random rotation
void ATile::GenerateBushes()
{
	if (!House && FMath::FRandRange(1, 2) < 2)
	{
		if (ToSpawnBush)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				FVector Location = this->GetActorLocation() + FVector(FMath::FRandRange(-450, 450), FMath::FRandRange(-450, 450), 0.0f);
				World->SpawnActor<AStaticMeshActor>(ToSpawnBush, Location, FRotator(0.0f, FMath::FRandRange(1, 180), 90.0f), spawnPar);
			}
		}
	}
}

bool ATile::ReturnHouse()
{
	return House;
}


