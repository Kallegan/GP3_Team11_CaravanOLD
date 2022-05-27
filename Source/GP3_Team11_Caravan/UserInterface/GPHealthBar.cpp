#include "GPHealthBar.h"
#include "GP3_Team11_Caravan/GPComponents/GPHealthComponent.h"
#include "Components/ProgressBar.h"


void UGPHealthBar::UpdateHealthBar() const
{
	if(OwnerEnemy.IsValid())
		HealthBar->SetPercent(OwnerEnemy->HealthComponent->GetCurrentHealth() / OwnerEnemy->HealthComponent->GetMaxHealth());
	else if(OwnerPlayer.IsValid())
		HealthBar->SetPercent(OwnerPlayer->HealthComponent->GetCurrentHealth() / OwnerPlayer->HealthComponent->GetMaxHealth());
		
}
