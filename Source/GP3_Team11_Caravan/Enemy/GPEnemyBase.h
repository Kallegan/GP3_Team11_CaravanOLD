#pragma once
#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GPEnemyBase.generated.h"

class UCapsuleComponent;
class UGPHealthComponent;
class UWidgetComponent;
class UGPHealthBar;


UCLASS()
class GP3_TEAM11_CARAVAN_API AGPEnemyBase : public APawn
{
	GENERATED_BODY()
	
public:		
	AGPEnemyBase();	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UFUNCTION(BlueprintNativeEvent)
	void OnDeathBPEvent();
	
private:
	UPROPERTY()
	UGPHealthBar* HealthBar = nullptr;
};