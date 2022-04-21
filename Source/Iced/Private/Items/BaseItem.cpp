// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseItem, All, All);

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Root Capsule Component");
	SetRootComponent(RootCapsuleComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetupAttachment(GetRootComponent());
}

void ABaseItem::ChangeCombatState()
{
	const auto ItemOwner = GetOwner<ACharacter>();
	if (!ItemOwner) { return; }

	bDoesInCombat = !bDoesInCombat;
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                  UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                  const UDamageType* DamageType,
                                  AActor* DamageCauser)
{
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
}
