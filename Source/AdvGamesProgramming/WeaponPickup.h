// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Components/TextRenderComponent.h"
#include "WeaponPickup.generated.h"


UENUM(BlueprintType)
enum class EWeaponPickupRarity : uint8
{
	SPECIAL,
	LEGENDARY,
	MASTER,
	RARE,
	COMMON
};

UCLASS()
class ADVGAMESPROGRAMMING_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EWeaponPickupRarity Rarity;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float BulletDamage;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MuzzleVelocity;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 MagazineSize;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float WeaponAccuracy;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float FireRate;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup(AActor* ActorThatPickedUp) override;
	UFUNCTION(BlueprintCallable)
	void OnGenerate() override;

	UPROPERTY(VisibleInstanceOnly)
	UTextRenderComponent* TextComponent;
	UPROPERTY(BlueprintReadWrite)
	float Luck;

private:
	void GenerateRandomBoolArray(int32 ArrayLength, int32 NumTrue, TArray<bool>& RandBoolArray);
	FString GenerateName(EWeaponPickupRarity WeaponRarity);
};
