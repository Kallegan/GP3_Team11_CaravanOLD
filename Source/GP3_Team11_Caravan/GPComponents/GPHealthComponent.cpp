#include "GPHealthComponent.h"
#include "GP3_Team11_Caravan/Enemy/GPEnemyBase.h"
#include "Engine.h"
#include "VectorTypes.h"
#include "GP3_Team11_Caravan/UserInterface/GPHealthBar.h"

UGPHealthComponent::UGPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	maxHealth = StartingHealth;
	currentHealth = StartingHealth;
}

void UGPHealthComponent::SetCurrentHealth(float HealAmount)
{
	FMath::Clamp(currentHealth += HealAmount, 0, maxHealth);
	UpdateHealthBar.Broadcast(MyOwner);	
}

void UGPHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();	
	MyOwner->SetCanBeDamaged(true);
	
	if(MyOwner)	
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UGPHealthComponent::TakeDamage);		
}

void UGPHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* EventInstigator, AActor* DamageCauser)
{
	if(Damage <= 0)
		return;

	FHitResult HitResult = FHitResult();
	FVector OwnerLocation = MyOwner->GetActorLocation();
	OwnerLocation = OwnerLocation -= FVector(0,0,80);
	FVector EnemyLocation = DamageCauser->GetActorLocation();
	FVector NormalizedDir = EnemyLocation-OwnerLocation;
	NormalizedDir.Normalize();
	
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(MyOwner);
	Params.AddIgnoredActor(DamageCauser);	
	
	if(GetWorld()->LineTraceSingleByChannel(HitResult, OwnerLocation, (NormalizedDir*MaxDistanceFromCover)+OwnerLocation, ECC_Visibility, Params))
	{
		DrawDebugLine(GetWorld(), OwnerLocation, (NormalizedDir*MaxDistanceFromCover)+OwnerLocation, FColor::Red,true);
		
		
		if (HitResult.GetActor()->ActorHasTag("FullCover"))
		{
			if(FMath::RandRange(1, 100) < FullCoverDodgeChance)			
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor in full cover evaded attack, %d percent chance"),FMath::RoundToInt(FullCoverDodgeChance))
				return;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Actor in full cover hit, took %d percent reduced damage"),FMath::RoundToInt(FullCoverDamageReduction))
			CoverState = FullCover;
			Damage -= Damage * FMath::Clamp(FullCoverDamageReduction, 1, 100) / 100;
		}		
		else if (HitResult.GetActor()->ActorHasTag("HalfCover"))
		{
			if(FMath::RandRange(1, 100) < HalfCoverDodgeChance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor in half cover evaded attack, %d percent chance"),FMath::RoundToInt(HalfCoverDodgeChance))
				return;
			}
				
			UE_LOG(LogTemp, Warning, TEXT("Actor in half cover hit, took %d percent reduced damage"),FMath::RoundToInt(HalfCoverDamageReduction))
			CoverState = HalfCover;
			Damage -= Damage * FMath::Clamp(HalfCoverDamageReduction, 1, 100) / 100;
		}		
	}
	else	
	{
			DrawDebugLine(GetWorld(), OwnerLocation, EnemyLocation, FColor::Green,true);
			CoverState = NoCover;
			UE_LOG(LogTemp, Warning, TEXT("no cover actor hit"))
	}	
		
	currentHealth = FMath::Clamp(currentHealth - Damage, 0.f, maxHealth);
	UpdateHealthBar.Broadcast(MyOwner);	
			
	if(currentHealth <= 0)	
		OnDeath();	
}

void UGPHealthComponent::OnDeath() const
{
	if(MyOwner)
		OnDeathEvent.Broadcast(MyOwner);		
}

