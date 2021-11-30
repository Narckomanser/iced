// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"

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

	void ABaseItem::OnComponentHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FHitResult HitResult = Hit;
	UE_LOG(LogBaseItem, Display, TEXT("HitResult: %s"), *HitResult.ToString());
}
