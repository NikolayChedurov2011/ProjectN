// N Chedurov All Rights Reserved


#include "ProjectN_PlayerCameraManager.h"

#include "Camera/CameraComponent.h"

AProjectN_PlayerCameraManager::AProjectN_PlayerCameraManager()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
}

UCameraComponent* AProjectN_PlayerCameraManager::GetCameraComponent() const
{
	return CameraComponent;
}
