// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_CharacterBase.h"
#include "GameFramework/Character.h"
#include "Inventory/ProjectN_ItemActor_Base.h"
#include "Net/UnrealNetwork.h"
#include "ProjectN_Statics.h"


void UProjectN_ItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_ItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UProjectN_ItemInstance, bIsEquipped);
	DOREPLIFETIME(UProjectN_ItemInstance, ItemActor);
	DOREPLIFETIME(UProjectN_ItemInstance, OwnerCharacter);
}

void UProjectN_ItemInstance::Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

const UItemStaticClass* UProjectN_ItemInstance::GetItemStaticClass() const
{
	return UProjectN_Statics::GetItemStaticData(ItemStaticDataClass);
}

void UProjectN_ItemInstance::OnEquip(AActor* Owner, const FName InSocket)
{
	OwnerCharacter = Cast<ACharacter>(Owner);
	
	if (UWorld* World = Owner->GetWorld())
	{
		const FTransform Transform;
		
		ItemActor = World->SpawnActorDeferred<AProjectN_ItemActor_Base>(GetItemStaticClass()->GetItemActorClass(), Transform, Owner);
		ItemActor->Init(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(Transform);

		if (USkeletalMeshComponent* SkeletalMeshComponent = OwnerCharacter ? OwnerCharacter->GetMesh() : nullptr)
		{
			ItemActor->AttachToComponent(SkeletalMeshComponent,  FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocket);
		}
	}
	
	bIsEquipped = true;
	ApplyItemAbilityAndEffects(OwnerCharacter);
}

void UProjectN_ItemInstance::OnUnEquip()
{
	if (IsValid(ItemActor))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
	
	bIsEquipped = false;
	
	RemoveItemAbilityAndEffects(OwnerCharacter);
	OwnerCharacter = nullptr;
}

void UProjectN_ItemInstance::OnDrop()
{
	if (IsValid(ItemActor))
	{
		ItemActor->OnDropped();
	}

	bIsEquipped = false;
}

void UProjectN_ItemInstance::OnRep_IsEquipped()
{
	
}

void UProjectN_ItemInstance::ApplyItemAbilityAndEffects(const AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return;
	}
	
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(InActor);

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
		
	for (const TSubclassOf<UGameplayAbility> Ability : GetItemStaticClass()->GetItemAbilities())
	{
		GameplayAbilitySpecHandles.Add(ASC->GiveAbility_Internal(Ability));
	}

	FGameplayEffectContextHandle EffectContext = ASCInterface->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(InActor);
	
	for (const TSubclassOf<UGameplayEffect> Effect : GetItemStaticClass()->GetItemEffects())
	{
		ActiveGameplayEffectHandles.Add(ASC->ApplyGamePlayEffectToSelf_Internal(Effect, EffectContext, 1.f));
	}
}

void UProjectN_ItemInstance::RemoveItemAbilityAndEffects(const ACharacter* InCharacter)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(InCharacter);
	
	if (GameplayAbilitySpecHandles.Num())
	{
		for (const FGameplayAbilitySpecHandle& GameplayAbilitySpecHandle : GameplayAbilitySpecHandles)
		{
			ASCInterface->GetAbilitySystemComponent()->ClearAbility(GameplayAbilitySpecHandle);
		}
	}
	if (ActiveGameplayEffectHandles.Num())
	{
		for (const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle : ActiveGameplayEffectHandles)
		{
			ASCInterface->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
		}
	}
	GameplayAbilitySpecHandles.Empty();
	ActiveGameplayEffectHandles.Empty();
}
