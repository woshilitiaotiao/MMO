
#pragma once

#include "CoreMinimal.h"

namespace SimpleActorAction
{
	class SIMPLEACTORBROWSING_API FSimpleZoom
	{
	public:
		FSimpleZoom();

		void Register(AActor* InTarget, float InDistance);
		void ZoomPositive(float InValue);
		void ZoomNegative(float InValue);
	protected:
		TWeakObjectPtr<AActor> Target;
		float Distance;
		FVector Location;
	};
}