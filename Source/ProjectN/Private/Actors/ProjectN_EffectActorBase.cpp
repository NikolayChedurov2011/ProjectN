// N Chedurov All Rights Reserved


#include "Actors/ProjectN_EffectActorBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AProjectN_EffectActorBase::AProjectN_EffectActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectN_EffectActorBase::ApplyEffect(AActor* TargetActor, TSubclassOf<UGameplayEffect> InEffectToApply)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr)
	{
		return;
	}

	check(InEffectToApply)

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectHandle = TargetASC->MakeOutgoingSpec(InEffectToApply, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectHandle.Data.Get());

	const bool bIsInfinite = GameplayEffectHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveGameplayEffectHandleMap.Add(ActiveGameplayEffectHandle, TargetASC);
	}
}

void AProjectN_EffectActorBase::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffect(TargetActor, InstantEffectToApply);
	}
	if (PeriodicEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffect(TargetActor, InstantEffectToApply);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffect(TargetActor, InstantEffectToApply);
	}

	/*	switch (InstantEffectApplicationPolicy)
	{
	case EEffectApplicationPolicy::ApplyOnOverlap : ApplyEffect(TargetActor, InstantEffectToApply);
		break;
	case EEffectApplicationPolicy::ApplyOnEndOverlap : ApplyEffect(TargetActor, InstantEffectToApply);
		break;
	case EEffectApplicationPolicy::DoNotApply : ApplyEffect(TargetActor, InstantEffectToApply);
		break;
	}*/
}

void AProjectN_EffectActorBase::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffect(TargetActor, InstantEffectToApply);
	}
	if (PeriodicEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffect(TargetActor, PeriodicEffectToApply);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffect(TargetActor, InfiniteEffectToApply);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))
		{
			return;
		}
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveGameplayEffectHandleMap)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		if (HandlesToRemove.Num())
		{
			for (const FActiveGameplayEffectHandle& HandleToRemove : HandlesToRemove)
			{
				ActiveGameplayEffectHandleMap.Remove(HandleToRemove);
			}
		}
	}
}
