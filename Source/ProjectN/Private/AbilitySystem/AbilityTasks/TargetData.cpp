// N Chedurov All Rights Reserved


#include "AbilitySystem/AbilityTasks/TargetData.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UTargetData* UTargetData::CreateTargetData(UGameplayAbility* OwningAbility)
{
	UTargetData* MyObj = NewAbilityTask<UTargetData>(OwningAbility);
	return MyObj;
}

void UTargetData::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendData();
	}
	else
	{
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddLambda([this](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag GameplayTag)
		{
			AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				ValidData.Broadcast(DataHandle);
			}
		});

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetData::SendData() const
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	FVector Direction = FVector::ZeroVector;
	FVector Location = FVector::ZeroVector;
	int32 ViewportX = 0;
	int32 ViewportY = 0;
	
	const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
		
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	PlayerController->DeprojectScreenPositionToWorld(ViewportX / 2, ViewportY / 2, Location, Direction);

	//UKismetSystemLibrary::DrawDebugSphere(this, Location, 5.0f, 8, FColor::Red, 7.f, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, Location, Location + Direction * 50.f, 20, FColor::Yellow, 7.f, 5.f);
	
	FVector FinalLocation = Location + Direction * 10000.f;

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult.Location = FinalLocation;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
