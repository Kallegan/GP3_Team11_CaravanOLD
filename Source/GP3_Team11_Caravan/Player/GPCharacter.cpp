// Fill out your copyright notice in the Description page of Project Settings.


#include "GPCharacter.h"
#include "GP3_Team11_Caravan/Weapon/GPWeaponBase.h"
#include "GP3_Team11_Caravan/UserInterface/GPHealthBar.h"
#include "GP3_Team11_Caravan/GPComponents/GPHealthComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGPCharacter::AGPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	

	HealthComponent = CreateDefaultSubobject<UGPHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathEvent.AddDynamic(this, &ThisClass::OnDeath);
	HealthComponent->UpdateHealthBar.AddDynamic(this, &ThisClass::UpdateHealthBar);
	HealthComponent->CoverState = NoCover;
	
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthWidgetComp->SetDrawAtDesiredSize(true);
	
}

// Called when the game starts or when spawned
void AGPCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthBar = Cast <UGPHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->SetOwnerPlayer(this);		
}

void AGPCharacter::OnDeath(AActor* OwningActor)
{
	OnDeathBPEvent();
}

void AGPCharacter::UpdateHealthBar(AActor* OwningActor)
{
	HealthBar->UpdateHealthBar();
}

// Called every frame
void AGPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


