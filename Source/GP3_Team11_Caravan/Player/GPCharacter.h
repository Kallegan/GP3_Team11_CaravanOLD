// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GP3_Team11_Caravan/Weapon/GPWeaponBase.h"
#include "GPCharacter.generated.h"

class AGPWeaponBase;
class UGPHealthComponent;
class UWidgetComponent;
class UGPHealthBar;

UCLASS()
class GP3_TEAM11_CARAVAN_API AGPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGPCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGPHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthWidgetComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDeath(AActor* OwningActor);

	UFUNCTION()
	virtual void UpdateHealthBar(AActor* OwningActor);


public:

	UPROPERTY(EditInstanceOnly)
	AGPWeaponBase* Weapon;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathBPEvent();

private:
	UPROPERTY()
	UGPHealthBar* HealthBar = nullptr;
};
