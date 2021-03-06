// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDObject(TEXT("/Game/Widgets/PlayerHUDWidget"));


	PlayerHUDClass = PlayerHUDObject.Class;


	if (PlayerHUDClass)
	{
		CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (CurrentPlayerHUDWidget)
		{
			CurrentPlayerHUDWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find CurrentPlayerHUDWidget"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find the PlayerHUDClass"))
	}

	if (CurrentPlayerHUDWidget)
	{
		HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
		RoundsRemainingText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsRemaining")));
		RoundsInMagazineText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("RoundsInMagazine")));
		KillsText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Kills")));
		DeathsText = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("Deaths")));
	}

}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
		if (Percent >= 0.66f)
		{
			HealthProgressBar->SetFillColorAndOpacity(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else if (Percent >= 0.33f)
		{
			HealthProgressBar->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{
			HealthProgressBar->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find the progress bar to update the health"))
	}
}

void APlayerHUD::HideHUD()
{
	if (CurrentPlayerHUDWidget)
	{
		CurrentPlayerHUDWidget->RemoveFromViewport();
	}
}

void APlayerHUD::ShowHUD()
{
	if (CurrentPlayerHUDWidget)
	{
		CurrentPlayerHUDWidget->AddToViewport();
	}
}

void APlayerHUD::SetAmmoText(int32 RoundsRemaining, int32 RoundsInMagazine)
{
	if (RoundsRemainingText && RoundsInMagazineText)
	{
		RoundsRemainingText->SetText(FText::FromString(FString::Printf(TEXT("%i"), RoundsRemaining)));
		RoundsInMagazineText->SetText(FText::FromString(FString::Printf(TEXT("%i"), RoundsInMagazine)));
	}
}

void APlayerHUD::SetKills(int32 Kills)
{
	if (KillsText)
	{
		KillsText->SetText(FText::FromString(FString::Printf(TEXT("%i"), Kills)));
	}
}

void APlayerHUD::SetDeaths(int32 Deaths)
{
	if (DeathsText)
	{
		DeathsText->SetText(FText::FromString(FString::Printf(TEXT("%i"), Deaths)));
	}
}