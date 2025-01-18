// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ProjectN_PlayerCameraManager.generated.h"

class UCameraComponent;

UCLASS(Abstract)
class PROJECTN_API AProjectN_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AProjectN_PlayerCameraManager();
};
