#include "GPEnemyBase.h"
#include "GP3_Team11_Caravan/UserInterface/GPHealthBar.h"
#include "GP3_Team11_Caravan/GPComponents/GPHealthComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGPEnemyBase::AGPEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));	
	CapsuleComp->SetCapsuleSize(42.f,96.f);
	CapsuleComp->SetupAttachment(RootComponent);
	
	
	HealthComponent = CreateDefaultSubobject<UGPHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathEvent.AddDynamic(this, &ThisClass::OnDeath);
	HealthComponent->UpdateHealthBar.AddDynamic(this, &ThisClass::UpdateHealthBar);
	HealthComponent->CoverState = NoCover;	

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(CapsuleComp, FAttachmentTransformRules::KeepRelativeTransform);
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComp->SetDrawAtDesiredSize(true);
	
}


// Called when the game starts or when spawned
void AGPEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	HealthBar = Cast <UGPHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->SetOwnerEnemy(this);	
}

// Called every frame
void AGPEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}


void AGPEnemyBase::OnDeathBPEvent_Implementation()
{
	
}

void AGPEnemyBase::OnDeath(AActor*)
{	
	OnDeathBPEvent();	
}

void AGPEnemyBase::UpdateHealthBar(AActor*)
{
	HealthBar->UpdateHealthBar();	
}
