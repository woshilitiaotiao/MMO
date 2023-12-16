// Fill out your copyright notice in the Description page of Project Settings.


#include "KneadingInterface.h"

IKneadingInterface::IKneadingInterface()
	:LegSize(0.f)
	,WaistSize(0.f)
	,ArmSize(0.f)
{

}

void IKneadingInterface::SetMeshPostion(USceneComponent* InMesh)
{
	if (InMesh)
	{
		FVector ComponentLocation = Location;
		ComponentLocation.Z += LegSize;

		InMesh->SetWorldLocation(ComponentLocation);
	}
}

// Add default functionality here for any IKneadingInterface functions that are not pure virtual.
void IKneadingInterface::SetLegSize(float InLegSize)
{
	LegSize = InLegSize;
}

void IKneadingInterface::SetWaistSize(float InWaistSize)
{
	WaistSize = InWaistSize;
}

void IKneadingInterface::SetArmSize(float InArmSize)
{
	ArmSize = InArmSize;
}

float IKneadingInterface::GetLegSize()
{
	return LegSize;
}
float IKneadingInterface::GetWaistSize()
{
	return WaistSize;
}
float IKneadingInterface::GetArmSize()
{
	return ArmSize;
}

void IKneadingInterface::InitKneadingLocation(const FVector& InLocation)
{
	Location = InLocation;
}