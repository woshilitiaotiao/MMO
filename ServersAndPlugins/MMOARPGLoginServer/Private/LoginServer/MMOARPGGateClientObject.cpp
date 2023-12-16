#include "MMOARPGGateClientObject.h"
#include "log/MMOARPGLoginServerLog.h"
#include "Protocol/ServerProtocol.h"

UMMOARPGGateClientObject::UMMOARPGGateClientObject()
	:Time(0.f)
{

}

void UMMOARPGGateClientObject::Init()
{
	Super::Init();
}

void UMMOARPGGateClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
	if (Time >= 1.f)
	{
		Time = 0.f;

		SIMPLE_PROTOCOLS_SEND(SP_GateStatusRequests);
	}
}

void UMMOARPGGateClientObject::Close()
{
	Super::Close();

}

void UMMOARPGGateClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_GateStatusResponses:
		{
			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_GateStatusResponses,GateStatus);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_GateStatusResponses]"));

			break;
		}
	}
}

FMMOARPGGateStatus& UMMOARPGGateClientObject::GetGateStatus()
{
	return GateStatus;
}

