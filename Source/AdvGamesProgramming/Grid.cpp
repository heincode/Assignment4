// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include <algorithm>
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Framework/Views/ITypedTableView.h"

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SideLength = 25;
	Scale = 1100;
	RoadLength = 300;
	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	RootComponent = LocationComponent;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	//Creates all of the Tiles and assigns them to the grid array
	
	if (GetGameInstance()->IsDedicatedServerInstance())
	{
		for (int32 i = 0; i < (SideLength * SideLength); i++)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters spawnPar;
				spawnPar.Owner = this;
				ATile* SpawnedTile = World->SpawnActor<ATile>(FindTilePos(i), FRotator(0.0f, 0.0f, 90.0f), spawnPar);
				SpawnedTile->ToSpawnHouse = House1;
				SpawnedTile->ToSpawnHouse2 = House2;
				SpawnedTile->ToSpawnHouse3 = House3;
				if (FMath::FRandRange(1, 12) < 3)
				{
					SpawnedTile->House = true;
				}
				else
				{
					SpawnedTile->House = false;
				}
				SpawnedTile->TileNo = i;
				TileGrid.Add(SpawnedTile);
				
			}
		}
		
		//Finds empty tile in the middle and sets as the road start then starts the road generation
		
			for (int i = 0; i < TileGrid.Num(); i++) {
				if (i < TileGrid.Num() / 2) {}
				else
				{
					if (StartFound) {}
					else
					{
						CurrentTile = i;
						if (TileGrid[i]->House) {}
						else
						{
							StartFound = true;
							RoadGen();
							//tells the tiles to generate trees
							FActorSpawnParameters spawnPar;
							spawnPar.Owner = this;
							GetWorld()->SpawnActor<AActor>(ObjectSetter, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 90.0f), spawnPar);
							for (ATile* tile : TileGrid)
							{
								tile->GenerateTrees();
							}
						}
					}
				}
			}
		}
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//finds the world position of a tile through its index number
FVector AGrid::FindTilePos(int index)
{
	float X = ((index / SideLength) + 1) * Scale;
	float Y = (FMath::TruncToFloat(index % SideLength) * Scale);
	return FVector(X, Y, 0.0f);
}

//generates the roads from the start position until roadlength is up
void AGrid::RoadGen()
{
	for (int32 i = 0; i < RoadLength; i++)
	{
		if (TileGrid[CurrentTile]->House)
		{
			FindNextRoad();
		}
		else
		{
			TileGrid[CurrentTile]->House = true;
			if (RoadToSpawn)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters spawnPar;
					spawnPar.Owner = this;
					World->SpawnActor<AStaticMeshActor>(RoadToSpawn, FindTilePos(CurrentTile), FRotator(0.0f, 0.0f, 0.0f), spawnPar);
					TileGrid[CurrentTile]->IsRoad = true;
					UE_LOG(LogTemp, Warning, TEXT("Road made"));
					FindNextRoad();
				}
			}
		}
	}
}

void AGrid::FindNextRoad()
{
	//Finds next possible positions
	if (TileGrid.IsValidIndex(CurrentTile - 1))
	{
		RoadOptions.Add(CurrentTile - 1);
	}
	if (TileGrid.IsValidIndex(CurrentTile + 1))
	{
		RoadOptions.Add(CurrentTile + 1);
	}
	if (TileGrid.IsValidIndex(CurrentTile + SideLength))
	{
		RoadOptions.Add(CurrentTile + SideLength);
	}
	if (TileGrid.IsValidIndex(CurrentTile - SideLength))
	{
		RoadOptions.Add(CurrentTile - SideLength);
	}

	//Removes positions that land on an occupied tile then selects one at random to be the next road
	if (RoadOptions.Num() != 0)
	{
		for (int i = 0; i < RoadOptions.Num(); i++)
		{
			if (TileGrid[RoadOptions[i]]->House)
			{
				RoadOptions.RemoveAt(i, 1, true);
			}
		}
		if (RoadOptions.Num() > 0)
		{
			CurrentTile = (RoadOptions[FMath::FRandRange(0, RoadOptions.Num())]);
			RoadOptions.Empty();
		}
	}
}

