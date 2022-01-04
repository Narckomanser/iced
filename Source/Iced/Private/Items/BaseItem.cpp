// Narckomanser's game


#include "BaseItem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseItem, All, All);

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Root Capsule Component");
	SetRootComponent(RootCapsuleComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetupAttachment(GetRootComponent());

	HitCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Hit Capsule Component");
	HitCapsuleComponent->SetupAttachment(GetRootComponent());
}

void ABaseItem::ChangeAttackState(USkeletalMeshComponent* MeshComp)
{
	const auto ItemOwner = GetOwner<ACharacter>();
	if (!ItemOwner) { return; }
	
	const auto OwnerMesh = ItemOwner->GetMesh();
	if (MeshComp != OwnerMesh) { return; }
	
	bDoesInAttack = !bDoesInAttack;
}

bool ABaseItem::CanTakeDamage() const
{
	return bDoesInAttack && (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) <= 0.f);
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
	if (!CanTakeDamage()) { return; }

	//TODO Take damage only when actor in attack, in battle stance
	//TODO set overlap enable only when attack in progress???
	if (const auto ItemOwner = GetOwner<APawn>())
	{
		//TODO calculate damage with some modifiers
		OtherActor->TakeDamage(DamageAmount, FPointDamageEvent{}, ItemOwner->GetController(), this);

		//TODO try replace timer with AnimNotify
		GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);
	}
}
