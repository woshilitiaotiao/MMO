// Copyright Epic Games, Inc. All Rights Reserved.
#include "MMOARPGGateServer.h"
#include "Log/MMOARPGGateServerLog.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleNetManage.h"
#include "ServerList.h"
#include "RequiredProgramMainCPPInclude.h"
#include "GateServer/MMOARPGGateServerObject.h"
#include "GateServer/MMOARPGdbClientObject.h"
#include "GateServer/MMOARPGCenterClientObject.h"

IMPLEMENT_APPLICATION(MMOARPGGateServer, "MMOARPGGateServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGGateServer, Display, TEXT("Hello World"));

	//I初始化配置表
	FSimpleNetGlobalInfo::Get()->Init();

	//II 创建服务器实例
	GateServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	dbClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	CenterClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	//III 注册反射类
	GateServer->NetworkObjectClass = UMMOARPGGateServerObject::StaticClass();
	dbClient->NetworkObjectClass = UMMOARPGdbClientObject::StaticClass();
	CenterClient->NetworkObjectClass = UMMOARPGCenterClientObject::StaticClass();

	//IV 初始化
	if (!GateServer->Init())
	{
		delete GateServer;
		UE_LOG(LogMMOARPGGateServer, Error, TEXT("GateServer Init fail."));
		return INDEX_NONE;
	}

	if (!dbClient->Init(11221))
	{
		delete dbClient;
		UE_LOG(LogMMOARPGGateServer, Error, TEXT("dbClient Init fail."));
		return INDEX_NONE;
	}

	if (!CenterClient->Init(11231))
	{
		delete dbClient;
		UE_LOG(LogMMOARPGGateServer, Error, TEXT("dbClient Init fail."));
		return INDEX_NONE;
	}

	//V 检测
	double LastTime = FPlatformTime::Seconds();
	while (!IsEngineExitRequested())
	{
		//休眠
		FPlatformProcess::Sleep(0.03f);

		//计算时间差
		double Now = FPlatformTime::Seconds();
		float DeltaSenconds = Now - LastTime;

		//每帧检查链接
		GateServer->Tick(DeltaSenconds);
		dbClient->Tick(DeltaSenconds);
		CenterClient->Tick(DeltaSenconds);

		LastTime = Now;
	}

	FSimpleNetManage::Destroy(dbClient);
	FSimpleNetManage::Destroy(GateServer);
	FSimpleNetManage::Destroy(CenterClient);
	FEngineLoop::AppExit();

	return 0;
}
