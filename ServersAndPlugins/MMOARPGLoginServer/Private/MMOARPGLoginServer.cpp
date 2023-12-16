// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGLoginServer.h"
#include "Log/MMOARPGLoginServerLog.h"
#include "RequiredProgramMainCPPInclude.h"
#include "Log/MMOARPGLoginServerLog.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleNetManage.h"
#include "ServerList.h"
#include "RequiredProgramMainCPPInclude.h"
#include "LoginServer/MMOARPGGateClientObject.h"
#include "LoginServer/MMOARPGLoginServerObject.h"
#include "LoginServer/MMOARPGdbClientObject.h"

IMPLEMENT_APPLICATION(MMOARPGLoginServer, "MMOARPGLoginServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGLoginServer, Display, TEXT("Hello World"));

	//I初始化配置表
	FSimpleNetGlobalInfo::Get()->Init();

	//II 创建服务器实例
	LoginServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	dbClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	GateClientA = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	//III 注册反射类
	LoginServer->NetworkObjectClass = UMMOARPGLoginServerObject::StaticClass();
	dbClient->NetworkObjectClass = UMMOARPGdbClientObject::StaticClass();
	GateClientA->NetworkObjectClass = UMMOARPGGateClientObject::StaticClass();

	//IV 初始化
	if (!LoginServer->Init())
	{
		delete LoginServer;
		UE_LOG(LogMMOARPGLoginServer, Error, TEXT("LoginServer Init fail."));
		return INDEX_NONE;
	}

	if (!dbClient->Init(11221))
	{
		delete dbClient;
		UE_LOG(LogMMOARPGLoginServer, Error, TEXT("dbClient Init fail."));
		return INDEX_NONE;
	}

	if (!GateClientA->Init(11222))
	{
		delete GateClientA;
		UE_LOG(LogMMOARPGLoginServer, Error, TEXT("GateClient Init fail."));
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
		LoginServer->Tick(DeltaSenconds);
		dbClient->Tick(DeltaSenconds);
		GateClientA->Tick(DeltaSenconds);

		LastTime = Now;
	}

	FSimpleNetManage::Destroy(LoginServer);
	FSimpleNetManage::Destroy(dbClient);
	FSimpleNetManage::Destroy(GateClientA);

	FEngineLoop::AppExit();

	return 0;
}
