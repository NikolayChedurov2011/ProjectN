// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "ProjectN_EffectActorBase.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	DoNotApply,
	ApplyOnOverlap,
	ApplyOnEndOverlap
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	DoNotRemove,
	RemoveOnEndOverlap
};

UCLASS()
class PROJECTN_API AProjectN_EffectActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectN_EffectActorBase();

protected:

	UFUNCTION(BlueprintCallable)
	void ApplyEffect(AActor* TargetActor, TSubclassOf<UGameplayEffect> InEffectToApply);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	TSubclassOf<UGameplayEffect> InstantEffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	TSubclassOf<UGameplayEffect> PeriodicEffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	TSubclassOf<UGameplayEffect> InfiniteEffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectApplicationPolicy PeriodicEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectRemovalPolicy InstantEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectRemovalPolicy PeriodicEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect To Apply")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectHandleMap;
};
