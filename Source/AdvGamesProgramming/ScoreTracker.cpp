// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreTracker.h"

// Sets default values
AScoreTracker::AScoreTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Deaths = 0;
	Kills = 0;

}

// Called when the game starts or when spawned
void AScoreTracker::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AScoreTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreTracker::AddDeath()
{
	Deaths++;
}

int AScoreTracker::GetDeaths()
{
	return Deaths;
}

