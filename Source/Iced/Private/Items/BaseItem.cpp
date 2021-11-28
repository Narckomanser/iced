// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseItem, All, All);

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	SetRootComponent(ItemMesh);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	CapsuleComponent->SetupAttachment(GetRootComponent());
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnActorHitHandle(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogBaseItem, Display, TEXT("ActorHit: SelfActor-%s, OtherActor-%s"), *SelfActor->GetName(), *OtherActor->GetName());
}
