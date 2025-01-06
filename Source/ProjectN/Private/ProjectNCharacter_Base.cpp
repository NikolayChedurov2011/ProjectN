// N Chedurov All Rights Reserved

#include "ProjectNCharacter_Base.h"

AProjectNCharacter_Base::AProjectNCharacter_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
}