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

	HitCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Hit Capsule Component");
	HitCapsuleComponent->SetupAttachment(GetRootComponent());
}

void ABaseItem::ChangeCombatState(USkeletalMeshComponent* MeshComp)
{
	const auto ItemOwner = GetOwner<ACharacter>();
	if (!ItemOwner) { return; }
	
	const auto OwnerMesh = ItemOwner->GetMesh();
	if (MeshComp != OwnerMesh) { return; }
	
	bDoesInCombat = !bDoesInCombat;
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
