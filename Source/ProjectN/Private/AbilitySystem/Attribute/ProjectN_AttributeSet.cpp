// N Chedurov All Rights Reserved


#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UProjectN_AttributeSet::UProjectN_AttributeSet()
{
	InitMaxHealth(150.f);
	InitHealth(150.f);
	InitMaxMana(100.f);
	InitMana(100.f);
	InitMaxStamina(200.f);
	InitStamina(150.f);
}

void UProjectN_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Current attribute values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}

	// Max attribute values
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > Data.EvaluatedData.Magnitude)
		{
			SetHealth(Data.EvaluatedData.Magnitude);
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		if (GetMana() > Data.EvaluatedData.Magnitude)
		{
			SetMana(Data.EvaluatedData.Magnitude);
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		if (GetStamina() > Data.EvaluatedData.Magnitude)
		{
			SetStamina(Data.EvaluatedData.Magnitude);
		}
	}
}

void UProjectN_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	/*if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			SetHealth(NewValue);
		}
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		if (GetMana() > NewValue)
		{
			SetMana(NewValue);
		}
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	if (Attribute == GetMaxStaminaAttribute())
	{
		if (GetStamina() > NewValue)
		{
			SetStamina(NewValue);
		}
	}*/
}

void UProjectN_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxMovementSpeed, COND_None, REPNOTIFY_Always);
}

void UProjectN_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceProperties.AbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceProperties.AbilitySystemComponent) && Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo.IsValid() && Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceProperties.AvatarActor = Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Props.SourceProperties.Controller = Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceProperties.Controller == nullptr && Props.SourceProperties.AvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceProperties.AvatarActor))
			{
				Props.SourceProperties.Controller = Pawn->GetController();
			}
		}
		if (Props.SourceProperties.Controller)
		{
			if (ACharacter* SourceCharacter = Cast<ACharacter>(Props.SourceProperties.Controller->GetPawn()))
			{
				Props.SourceProperties.Character = SourceCharacter;
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetProperties.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetProperties.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetProperties.Character = Cast<ACharacter>(Props.TargetProperties.AvatarActor);
		Props.TargetProperties.AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetProperties.AvatarActor);
	}
}

void UProjectN_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Health, OldHealth);
}

void UProjectN_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Health, OldMaxHealth);
}

void UProjectN_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Mana, OldMana);
}

void UProjectN_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxMana, OldMaxMana);
}

void UProjectN_AttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxMovementSpeed, OldMaxMovementSpeed);
}

void UProjectN_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Stamina, OldStamina);
}

void UProjectN_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxStamina, OldMaxStamina);
}
