#include "Core/SimpleBrowse.h"

namespace SimpleActorAction
{
	FSimpleBrowse::FSimpleBrowse()
		:bRotateCharacter(false)
		, Controller(NULL)
		, Target(NULL)
	{

	}

	FSimpleBrowse::FSimpleBrowse(APlayerController* InController ,AActor* InTarget)
		:bRotateCharacter(false)
		,Controller(InController)
		,Target(InTarget)
	{

	}

	void FSimpleBrowse::Register(APlayerController* InController, AActor* InTarget)
	{
		Controller = InController;
		Target = InTarget;
	}

	//Monitored by main thread 
	void FSimpleBrowse::Tick(float DeltaTime)
	{
		if (Controller && Target.IsValid())
		{
			if (bRotateCharacter)
			{
				FRotator CurrentRot = Target->GetActorRotation();

				FVector2D NewMousePos;
				Controller->GetMousePosition(NewMousePos.X, NewMousePos.Y);
				float RotSpeed = (NewMousePos.X - MousePos.X) * -1.f;

				CurrentRot.Yaw += RotSpeed * 1.f;
				Target->SetActorRotation(CurrentRot);

				MousePos = NewMousePos;
			}
			else
			{	
				Controller->GetMousePosition(MousePos.X, MousePos.Y);
			}
		}
	}

	TStatId FSimpleBrowse::GetStatId() const
	{
		return TStatId();
	}

	void FSimpleBrowse::ResetTarget(AActor* InNewTarget)
	{
		Target = InNewTarget;
	}

	void FSimpleBrowse::OpenBrowsing()
	{
		bRotateCharacter = true;
	}

	void FSimpleBrowse::EndBrowsing()
	{
		bRotateCharacter = false;
	}
}