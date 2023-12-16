
#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

namespace SimpleActorAction
{
	class SIMPLEACTORBROWSING_API FSimplePanelMove : public FTickableGameObject
	{
	public:
		FSimplePanelMove();

		//Monitored by main thread 
		virtual void Tick(float DeltaTime);
		virtual TStatId GetStatId() const;

		void Register(APlayerController* InController, AActor* InTarget,float InDistance);

		void BeginMove();
		void EndMove();

		void ResetTarget(AActor* InNewTarget);
	protected:
		bool bMove;
		APlayerController* Controller;
		TWeakObjectPtr<AActor> Target;
		FVector2D MousePos;
		FVector Location;
		float Distance;
	};
}