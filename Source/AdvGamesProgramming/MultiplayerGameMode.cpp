// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "ProcedurallyGeneratedMap.h"
#include "Engine/Engine.h"
#include "GameFramework/HUD.h"
#include "TimerManager.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.h"

void AMultiplayerGameMode::Respawn(AController* Controller)
{
	if (Controller)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Respawning")));
		}

		//Hide the hud
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerController->GetPawn());
			if (Character && !Character->IsLocallyControlled())
			{
				Character->SetPlayerHUDVisibility(false);
			}
			else if (Character && Character->IsLocallyControlled())
			{
				APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
				if (PlayerHUD)
				{
					PlayerHUD->HideHUD();
				}
			}
		}

		//Remove the player
		APawn* Pawn = Controller->GetPawn();
		if (Pawn)
		{
			Pawn->SetLifeSpan(0.1f);
		}

		//Trigger the respawn process

		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, TEXT("TriggerRespawn"), Controller);
		GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, 5.0f, false);
	}
}

void AMultiplayerGameMode::TriggerRespawn(AController* Controller)
{
	if (Controller)
	{
		AActor* SpawnPoint = ChoosePlayerStart(Controller);
		if (SpawnPoint)
		{

			APawn* SpawnedPlayer = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, FVector(FMath::FRandRange(600, 27800), FMath::FRandRange(-400, 27000),20.0f), SpawnPoint->GetActorRotation());
			if (SpawnedPlayer)
			{
				Controller->Possess(SpawnedPlayer);
			}
		}
	}

	//Show the hud
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerController->GetPawn());
		if (Character && !Character->IsLocallyControlled())
		{
			Character->SetPlayerHUDVisibility(true);
		}
		else if (Character && Character->IsLocallyControlled())
		{
			APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (PlayerHUD)
			{
				PlayerHUD->ShowHUD();
				PlayerHUD->SetPlayerHealthBarPercent(1.0f);
			}

			Character->ResetModelVisibility();
		}
	}
}