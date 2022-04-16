// Narckomanser's game


#include "Notifies/BaseAnimNotify.h"

void UBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(MeshComp);

	Super::Notify(MeshComp, Animation, EventReference);
}
