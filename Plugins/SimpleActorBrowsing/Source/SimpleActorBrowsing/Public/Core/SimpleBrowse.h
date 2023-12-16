
#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

namespace SimpleActorAction 
{
	class SIMPLEACTORBROWSING_API FSimpleBrowse : public FTickableGameObject
	{
	public:
		FSimpleBrowse(APlayerController* InController, AActor* InTarget);
		FSimpleBrowse();

		//Monitored by main thread 
		virtual void Tick(float DeltaTime);
		virtual TStatId GetStatId() const;

		void Register(APlayerController* InController, AActor* InTarget);

		void OpenBrowsing();
		void EndBrowsing();

		void ResetTarget(AActor* InNewTarget);
	protected:
		bool bRotateCharacter;
		APlayerController* Controller;
		TWeakObjectPtr<AActor> Target;
		FVector2D MousePos;
	};
}