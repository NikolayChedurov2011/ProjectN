// N Chedurov All Rights Reserved


#include "UI/Widgets/Description/ProjectN_EntryDescription.h"

#include "Components/SizeBox.h"

FVector2D UProjectN_EntryDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize(); 
}
