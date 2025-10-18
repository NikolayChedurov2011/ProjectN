// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_WeaponActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
//#include "Components/CapsuleComponent.h"

/*void AProjectN_WeaponActor::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	TMap<FGameplayTag, float> DamageTypes = Cast<UWeaponItemStaticClass>(ItemInstance->GetItemStaticClass())->GetWeaponDamageTypes();
	
	
	if (GetOwningActorFromActorInfo()->Implements<UInventoryInterface>() && IsValid(DamageEffect))
	{
		AProjectN_WeaponActor* Weapon = IInventoryInterface::Execute_GetEquippedWeaponForSlot(GetOwningActorFromActorInfo(), RequiredSlot);
		
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Weapon);
		ContextHandle.SetAbility(this);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), ContextHandle);
		
		AssignDamageTypes(SpecHandle, IInventoryInterface::Execute_GetWeaponDamageTypes(GetOwningActorFromActorInfo(), RequiredSlot));

		Weapon->SetDamageEffectHandle(SpecHandle);
	}
}*/

AProjectN_WeaponActor::AProjectN_WeaponActor() : Super()
{
	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	//CapsuleComponent->SetupAttachment(GetRootComponent());
	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectN_WeaponActor::OnWeaponOverlap);
}

void AProjectN_WeaponActor::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || !DamageEffectHandle.Data)
	{
		return;
	}
	
	if(HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());
		}
	}
	else
	{
		SpawnImpactSoundAndEffect();
	}
}

void AProjectN_WeaponActor::SpawnImpactSoundAndEffect() const
{
	
}
