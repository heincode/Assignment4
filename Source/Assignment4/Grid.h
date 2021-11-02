// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


UCLASS()
class ASSIGNMENT4_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

	UPROPERTY(EditAnywhere)
		int32 SideLength;
	UPROPERTY(EditAnywhere)
		int32 RoadLength;
	UPROPERTY(BlueprintReadOnly)
		TArray<ATile*> TileGrid;
	int32 Scale;
	bool StartFound;
	int CurrentTile;
	TArray<int32> RoadOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> RoadToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> ObjectSetter;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector FindTilePos(int index);
	void RoadGen();
	void FindNextRoad();
};

