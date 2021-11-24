// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"

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
