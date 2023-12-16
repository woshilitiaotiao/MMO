#include "Core\SimplePanelMove.h"

namespace SimpleActorAction
{
	FSimplePanelMove::FSimplePanelMove()
		:bMove(false)
		,Distance(INDEX_NONE)
	{

	}

	void FSimplePanelMove::Tick(float DeltaTime)
	{
		if (Controller && Target.IsValid())
		{
			if (bMove)
			{
				float Max = Location.Z + Distance / 2.f;
				float Min = Location.Z - Distance / 2.f;

				FVector CurrentPos = Target->GetActorLocation();

				FVector2D NewMousePos;
				Controller->GetMousePosition(NewMousePos.X, NewMousePos.Y);

				float RotSpeed = (NewMousePos.Y - MousePos.Y) * -1.f;

				CurrentPos.Z += RotSpeed * 1.f;
				if (CurrentPos.Z < Max && CurrentPos.Z > Min)
				{
					Target->SetActorLocation(CurrentPos);
				}

				MousePos = NewMousePos;
			}
			else
			{
				Controller->GetMousePosition(MousePos.X, MousePos.Y);
			}
		}
	}

	TStatId FSimplePanelMove::GetStatId() const
	{
		return TStatId();
	}

	void FSimplePanelMove::Register(APlayerController* InController, AActor* InTarget, float InDistance)
	{
		Controller = InController;
		Target = InTarget;
		Distance = InDistance;

		Location = InTarget->GetActorLocation();
	}

	void FSimplePanelMove::BeginMove()
	{
		bMove = true;
	}

	void FSimplePanelMove::EndMove()
	{
		bMove = false;
	}

	void FSimplePanelMove::ResetTarget(AActor* InNewTarget)
	{
		Target = InNewTarget;
	}
}

