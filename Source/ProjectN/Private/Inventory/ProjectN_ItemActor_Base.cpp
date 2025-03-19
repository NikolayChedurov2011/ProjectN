// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemActor_Base.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_GameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/SphereComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/ProjectN_ItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

AProjectN_ItemActor_Base::AProjectN_ItemActor_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectN_ItemActor_Base::OnItemOverlap);
}

void AProjectN_ItemActor_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectN_ItemActor_Base, ItemInstance);
	DOREPLIFETIME(AProjectN_ItemActor_Base, ItemState);
}

bool AProjectN_ItemActor_Base::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bIsWroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bIsWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	
	return bIsWroteSomething;
}

void AProjectN_ItemActor_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectN_ItemActor_Base::Init(UProjectN_ItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

void AProjectN_ItemActor_Base::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::None:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;

	case EItemState::Dropped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
		
	case EItemState::Equipped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;

	default:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	}
}

void AProjectN_ItemActor_Base::OnEquipped()
{
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectN_ItemActor_Base::OnUnequipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectN_ItemActor_Base::OnDropped()
{
	ItemState = EItemState::Dropped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	if (GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = GetOwner()->GetActorForwardVector();

		const float DropItemForwardDist = 100.f;
		const float DropItemDownDist = 1000.f;

		const FVector TraceStart = Location + Forward * DropItemForwardDist;
		const FVector TraceEnd = TraceStart - FVector::UpVector * DropItemDownDist;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };
		FHitResult Hit;
		
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), false, ActorsToIgnore, EDrawDebugTrace::Persistent, Hit, true))
		{
			if (Hit.bBlockingHit)
			{
				SetActorLocation(Hit.Location);
				return;
			}
		}

		SetActorLocation(TraceEnd);
	}
}

void AProjectN_ItemActor_Base::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = ProjectNGameplayTags::InventoryTag_Equip;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, ProjectNGameplayTags::InventoryTag_Equip, EventPayload);
	}
}
