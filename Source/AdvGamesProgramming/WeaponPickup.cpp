// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

FString AWeaponPickup::Prefix[4][4] = { {"White", "Useless", "Unimpressive", "Boring"},
							{"Blue", "Average", "Mediocre", "Okay-ish"},
							{"Purple", "Higher", "Good", "Almost-Legendary"},
							{"Golden", "Legendary", "Pog", "Famed"} };
FString AWeaponPickup::Suffix[4] = { "Shooting", "Pog", "Killing", "Unknown" };

void AWeaponPickup::OnGenerate()
{
	APickup::OnGenerate();

	float RarityValue = FMath::RandRange(0.0f, 1.0f);
	RarityValue = (RarityValue + Luck) / (1.0f + Luck);
	TArray<bool> RandBoolArray;

	if (RarityValue <= 0.50f)
	{
		Rarity = EWeaponPickupRarity::COMMON;
		GenerateRandomBoolArray(4, 0, RandBoolArray);
	}
	else if (RarityValue <= 0.80f)
	{
		Rarity = EWeaponPickupRarity::RARE;
		GenerateRandomBoolArray(4, 1, RandBoolArray);
	}
	else if (RarityValue <= 0.95f)
	{
		Rarity = EWeaponPickupRarity::MASTER;
		GenerateRandomBoolArray(4, 3, RandBoolArray);
	}
	else
	{
		Rarity = EWeaponPickupRarity::LEGENDARY;
		GenerateRandomBoolArray(4, 4, RandBoolArray);
		if (FMath::RandRange(0.0f, 1.0f) <= Luck)
		{
			Rarity = EWeaponPickupRarity::SPECIAL;
		}
	}

	//Assign the good or bad weapon characteristics based on the result of the random boolean array.
	BulletDamage = (RandBoolArray[0] ? FMath::RandRange(15.0f, 30.0f) : FMath::RandRange(2.0f, 15.0f));
	MuzzleVelocity = (RandBoolArray[1] ? FMath::RandRange(10000.0f, 20000.0f) : FMath::RandRange(5000.0f, 10000.0f));
	MagazineSize = (RandBoolArray[2] ? FMath::RandRange(20, 100) : FMath::RandRange(1, 20));
	WeaponAccuracy = (RandBoolArray[3] ? FMath::RandRange(0.95f, 1.0f) : FMath::RandRange(0.8f, 0.95f));

	if (Rarity == EWeaponPickupRarity::SPECIAL)
	{
		GenerateRandomBoolArray(4, 1, RandBoolArray);
		BulletDamage = (RandBoolArray[0] ? FMath::RandRange(50.0f, 100.0f) : BulletDamage);
		MuzzleVelocity = (RandBoolArray[1] ? FMath::RandRange(50000.0f, 100000.0f) : MuzzleVelocity);
		MagazineSize = (RandBoolArray[2] ? FMath::RandRange(250, 999) : MagazineSize);
		FireRate = (RandBoolArray[3] ? FMath::RandRange(0.1f, 0.15f) : FireRate);
	}
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponPickup, Text);
	DOREPLIFETIME(AWeaponPickup, Rarity);
	DOREPLIFETIME(AWeaponPickup, BulletDamage);
	DOREPLIFETIME(AWeaponPickup, MuzzleVelocity);
	DOREPLIFETIME(AWeaponPickup, MagazineSize);
	DOREPLIFETIME(AWeaponPickup, WeaponAccuracy);
	DOREPLIFETIME(AWeaponPickup, FireRate);
}

void AWeaponPickup::GenerateRandomBoolArray(int32 ArrayLength, int32 NumTrue, TArray<bool>& RandBoolArray)
{
	for (int32 i = 0; i < ArrayLength; i++)
	{
		//Ternary Condition
		RandBoolArray.Add(i < NumTrue ? true : false);
	}

	//Card Shuffling Algorithm
	for (int32 i = 0; i < RandBoolArray.Num(); i++)
	{
		int32 RandIndex = FMath::RandRange(0, RandBoolArray.Num() - 1);
		bool Temp = RandBoolArray[i];
		RandBoolArray[i] = RandBoolArray[RandIndex];
		RandBoolArray[RandIndex] = Temp;
	}
}

FString AWeaponPickup::GenerateName(EWeaponPickupRarity WeaponRarity)
{
	float Rand = FMath::RandRange(0.0f, 1.0f);
	FString GunName;

	int index;
	switch (WeaponRarity)
	{
	case EWeaponPickupRarity::COMMON:
		index = 0;
		break;
	case EWeaponPickupRarity::RARE:
		index = 1;
		break;
	case EWeaponPickupRarity::MASTER:
		index = 2;
		break;
	case EWeaponPickupRarity::LEGENDARY:
		index = 3;
		break;
	case EWeaponPickupRarity::SPECIAL:
		index = 4;
		break;
	}

	if (WeaponRarity != EWeaponPickupRarity::SPECIAL)
	{
		GunName += (FMath::RandBool()) ? Prefix[index][FMath::RandRange(0, 4)] + " " : "";
		GunName += (FMath::RandBool()) ? "Gun of " + Suffix[FMath::RandRange(0, 4)] : "Gun";
	}
	else
	{
		GunName = (BulletDamage >= 50.0f) ? "Not-so-Special Gun with Deadly Bullets" :
				   (MuzzleVelocity >= 50000.0f) ? "Laser Shooter" :
				   (MagazineSize >= 250) ? "Mr. \"Never Reload\"" :
				   (FireRate <= 0.15f) ? "Manual Minigun" : GunName;
	}

	return GunName;
}

void AWeaponPickup::BlueprintSetText()
{
	TextComponent = FindComponentByClass<UTextRenderComponent>();
	if (TextComponent)
	{
		Text = GenerateName(Rarity) + "\n"
			"Bullet Damage: " + FString::Printf(TEXT("%d"), FMath::FloorToInt(BulletDamage)) + "\n" +
			"Muzzle Velocity: " + FString::Printf(TEXT("%d"), FMath::FloorToInt(MuzzleVelocity)) + "\n" +
			"Magazine Size: " + FString::Printf(TEXT("%d"), MagazineSize) + "\n" +
			"Weapon Accuracy: " + FString::Printf(TEXT("%d"), FMath::FloorToInt(WeaponAccuracy * 100)) + "%";
		if (FireRate <= 0.15f && FireRate != 0)
		{
			Text += "Passive\n\"Cookie Clicker\": Increased Fire Rate";
		}

		TextComponent->SetText(Text);
		TextComponent->SetVisibility(true);
	}
}
