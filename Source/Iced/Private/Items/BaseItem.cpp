// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"

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

