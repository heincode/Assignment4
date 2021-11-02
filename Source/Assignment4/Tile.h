// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS(BlueprintType)
class ASSIGNMENT4_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();
	USceneComponent* LocationComponent;
	float Rot;
	float RoofHeight;
	UPROPERTY(BlueprintReadWrite)
		bool House;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStaticMeshActor> ToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateBuilding();
	UFUNCTION(BlueprintCallable)
		void GenerateTrees();
	bool ReturnHouse();
};
