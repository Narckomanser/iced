// Narckomanser's game


#include "Notifies/BaseAnimNotify.h"

void UBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	
	Super::Notify(MeshComp, Animation);
}
