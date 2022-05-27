#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h" //TEST
#include "GPHealthComponent.generated.h"


enum CurrentCoverState : int16
{
	NoCover = 0,
	HalfCover = 1,
	FullCover = 2,
};

//Delegate to signal actor death.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHealthBar, AActor*, OwningActor);

class UGPWidget;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GP3_TEAM11_CARAVAN_API UGPHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UGPHealthComponent();
public:	

	CurrentCoverState CoverState;
	
	UPROPERTY()
	FDeathEvent OnDeathEvent;
	UPROPERTY()
	FUpdateHealthBar UpdateHealthBar;
	
	//Test data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable)
	UDataTable* HealthDataTable;
	//ENDTEST

	UPROPERTY()
	AActor* MyOwner = nullptr;	

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float StartingHealth = 100.f;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() { return currentHealth; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float HealAmount);

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return maxHealth; }
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float AddMaxToCurrentHp) { maxHealth += AddMaxToCurrentHp; }

	UFUNCTION(BlueprintCallable)
	int GetCurrentCoverState() const { return CoverState; };

	UPROPERTY(EditDefaultsOnly, Category = "Cover")
	float MaxDistanceFromCover = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Cover")
	float FullCoverDodgeChance = 60.f;
	UPROPERTY(EditDefaultsOnly, Category = "Cover")
	float HalfCoverDodgeChance = 30.f;

	//How many percent of the damage taken taken away from total damage behind cover.
	UPROPERTY(EditDefaultsOnly, Category = "Cover")
	float FullCoverDamageReduction = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Cover")
	float HalfCoverDamageReduction = 25.f;

protected:
	
	virtual void BeginPlay() override;	
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION()
	void OnDeath() const;
	
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp;

private:	
	float currentHealth;
	float maxHealth;	
};