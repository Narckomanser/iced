// Narckomanser's game


#include "Develop/DamageSphereActor.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ADamageSphereActor::ADamageSphereActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADamageSphereActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 64, Color);

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		GetActorLocation(),
		Radius,
		nullptr,
		{},
		this,
		nullptr,
		DoFullDamage);
}
