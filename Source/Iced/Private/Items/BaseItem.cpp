// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseItem, All, All);

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	SetRootComponent(CapsuleComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetupAttachment(GetRootComponent());
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	//if (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) > 0) { return; }
	if (!CanAttack()) { return; }

	//TODO Take damage only when actor in attack, in battle stance
	//TODO set overlap enable only when attack in progress???
	if (const auto ItemOwner = Cast<APawn>(GetOwner()))
	{
		//TODO calculate damage with some modifiers
		OtherActor->TakeDamage(DamageAmount, FPointDamageEvent{}, ItemOwner->GetController(), this);

		//TODO try replace timer with AnimNotify
		//GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);
	}
}

void ABaseItem::OnActorHitHandle(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogBaseItem, Display, TEXT("Damaged actor: %s"), *OtherActor->GetName());
}
