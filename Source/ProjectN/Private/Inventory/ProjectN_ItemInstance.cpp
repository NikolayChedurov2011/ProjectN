// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_CharacterBase.h"
#include "GameFramework/Character.h"
#include "Inventory/ProjectN_ItemActor_Base.h"
#include "Net/UnrealNetwork.h"
#include "ProjectN_Statics.h"

/****************************
 *  Item Instance
 ****************************/

void UProjectN_ItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_ItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UProjectN_ItemInstance, ItemStack);
}

void UProjectN_ItemInstance::Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

const UItemStaticClass* UProjectN_ItemInstance::GetItemStaticClass() const
{
	return UProjectN_Statics::GetItemStaticData(ItemStaticDataClass);
}

void UProjectN_ItemInstance::InitItemStack(const int32 InitiateStack)
{
	ItemStack = InitiateStack;
}

void UProjectN_ItemInstance::AddItemStack(const int32 StackToAdd)
{
	ItemStack +=  StackToAdd;
}

int32 UProjectN_ItemInstance::UseItem()
{
	return --ItemStack;
}

/****************************
 *  Equippable Item Instance
 ****************************/

void UProjectN_EquippableItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_EquippableItemInstance, bIsEquipped);
	DOREPLIFETIME(UProjectN_EquippableItemInstance, ItemActor);
	DOREPLIFETIME(UProjectN_EquippableItemInstance, OwnerCharacter);
}

void UProjectN_EquippableItemInstance::OnEquip(AActor* Owner, const FName InSocket, const FGameplayTag& InputTag)
{
	OwnerCharacter = Cast<ACharacter>(Owner);
	
	if (UWorld* World = Owner->GetWorld())
	{
		FTransform Transform;
		
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
	ApplyItemAbilityAndEffects(OwnerCharacter, InputTag);
}

void UProjectN_EquippableItemInstance::OnUnEquip()
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

void UProjectN_EquippableItemInstance::OnDrop()
{
	if (IsValid(ItemActor))
	{
		ItemActor->OnDropped();
	}

	bIsEquipped = false;
}

void UProjectN_EquippableItemInstance::OnRep_IsEquipped()
{
	
}

void UProjectN_EquippableItemInstance::ApplyItemAbilityAndEffects(const AActor* InActor, const FGameplayTag& InputTag)
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
		
	for (const TSubclassOf<UGameplayAbility> Ability : Cast<UEquippableItemStaticClass>(GetItemStaticClass())->GetItemAbilitiesToAdd())
	{
		GameplayAbilitySpecHandles.Add(ASC->AddAbility(Ability, InputTag));
	}
	
	FGameplayEffectContextHandle EffectContext = ASCInterface->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(InActor);
	
	for (const TSubclassOf<UGameplayEffect> Effect : Cast<UEquippableItemStaticClass>(GetItemStaticClass())->GetItemEffects())
	{
		ActiveGameplayEffectHandles.Add(ASC->ApplyGamePlayEffectToSelf_Internal(Effect, EffectContext, 1.f));
	}

	// Apply item attributes
	for (const TTuple<FGameplayTag, float> Attribute : Cast<UEquippableItemStaticClass>(GetItemStaticClass())->GetItemBonusAttributes())
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, Attribute.Value);
	}
}

void UProjectN_EquippableItemInstance::RemoveItemAbilityAndEffects(const ACharacter* InCharacter)
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

	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	// Discard item attributes
	for (const TTuple<FGameplayTag, float> Attribute : Cast<UEquippableItemStaticClass>(GetItemStaticClass())->GetItemBonusAttributes())
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, -Attribute.Value);
	}
}

FVector UProjectN_EquippableItemInstance::GetItemSocketLocationForProjectile() const
{
	return ItemActor? ItemActor->GetWeaponSocketLocationForProjectile() : FVector::ZeroVector;
}