// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AvatarInfoInterface.generated.h"

enum class EItemSlot : uint8;
struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UAvatarInfoInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTN_API IAvatarInfoInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel() const;
	
	UFUNCTION(BlueprintNativeEvent)
	FVector GetWeaponSocketLocation(const EItemSlot ItemSlot) const;

	UFUNCTION(BlueprintNativeEvent)
	FVector GetAvatarSocketLocation(const FName SocketName) const;
};
