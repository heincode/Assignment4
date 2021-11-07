// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.h"
#include "Tile.generated.h"


UCLASS(BlueprintType)
class ADVGAMESPROGRAMMING_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();
	USceneComponent* LocationComponent;
	float Rot;
	float RoofHeight;
	int TileNo;
	bool IsRoad;
	UPROPERTY(BlueprintReadWrite)
		bool House;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawnBush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawnHouse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawnHouse2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawnHouse3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeaponPickup> ToSpawnGun;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateBuilding();
	void GenerateTrees();
	void GenerateBushes();
	bool ReturnHouse();
};
