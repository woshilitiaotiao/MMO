// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "MMOARPGCenterServer.h"
#include "Log/MMOARPGCenterServerLog.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleNetManage.h"
#include "ServerList.h"
#include "RequiredProgramMainCPPInclude.h"
#include "CenterServer/MMOARPGCenterServerObject.h"
#include "CenterServer/MMOARPGdbClientObject.h"

IMPLEMENT_APPLICATION(MMOARPGCenterServer, "MMOARPGCenterServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGCenterServer, Display, TEXT("Hello World"));

	//I初始化配置表
	FSimpleNetGlobalInfo::Get()->Init();

	//II 创建服务器实例
	CenterServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	dbClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	
	//III 注册反射类
	CenterServer->NetworkObjectClass = UMMOARPGCenterServerObject::StaticClass();
	dbClient->NetworkObjectClass = UMMOARPGdbClientObject::StaticClass();

	//IV 初始化
	if (!CenterServer->Init())
	{
		delete CenterServer;
		UE_LOG(LogMMOARPGCenterServer, Error, TEXT("GateServer Init fail."));
		return INDEX_NONE;
	}

	if (!dbClient->Init(11221))
	{
		delete dbClient;
		UE_LOG(LogMMOARPGCenterServer, Error, TEXT("dbClient Init fail."));
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
		CenterServer->Tick(DeltaSenconds);
		dbClient->Tick(DeltaSenconds);

		LastTime = Now;
	}

	FSimpleNetManage::Destroy(dbClient);
	FSimpleNetManage::Destroy(CenterServer);
	FEngineLoop::AppExit();

	return 0;
}
