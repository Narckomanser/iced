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
	if (const auto ItemOwner = Cast<APawn>(GetOwner()))
	{
		//TODO Take damage only when actor in attack, in battle stance. add delay to handle multiple events in single attack
		//TODO calculate damage with some modifiers, 
		OtherActor->TakeDamage(10.f, FPointDamageEvent{}, ItemOwner->GetController(), this);
		UE_LOG(LogBaseItem, Warning, TEXT("send event to %s"), *OtherActor->GetName());
	}
	else
	{
		UE_LOG(LogBaseItem, Warning, TEXT("Item doesn't have an owner"));
	}
}
