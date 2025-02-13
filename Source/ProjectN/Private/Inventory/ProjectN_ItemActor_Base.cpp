// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemActor_Base.h"

#include "AbilitySystemBlueprintLibrary.h"
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

void AProjectN_ItemActor_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectN_ItemActor_Base::Init(UProjectN_ItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
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
	
	//GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//UE_LOG(LogTemp, Warning, TEXT("Attached Actor: %s"), *GetAttachParentActor()->GetName());
	if (IsValid(GetAttachParentActor()))
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Warning, TEXT("Detached!"));
	}
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
	FGameplayEventData EventPayload;
	EventPayload.OptionalObject = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OverlapEventTag, EventPayload);
}
