// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGdbServer.h"
#include "bdServer/MyqlConfig.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleNetManage.h"
#include "bdServer/MMOARPGServerObject.h"
#include "log/MMOARPGdbServerLog.h"
#include "RequiredProgramMainCPPInclude.h"
#include "SimpleHTTPManage.h"

IMPLEMENT_APPLICATION(MMOARPGdbServer, "MMOARPGdbServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGdbServer, Display, TEXT("Hello World"));

	//I初始化配置表
	FSimpleMysqlConfig::Get()->Init();
	FSimpleNetGlobalInfo::Get()->Init();

	//II 创建服务器实例
	FSimpleNetManage *dbServer = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN,ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	//III 注册反射类
	FSimpleChannel::SimpleControllerDelegate.BindLambda(
	[]()->UClass*
	{
		return UMMOARPGServerObject::StaticClass();
	});

	//IV 初始化
	if (!dbServer->Init())
	{
		delete dbServer;
		UE_LOG(LogMMOARPGdbServer, Error, TEXT("dbServer Init fail."));
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
		dbServer->Tick(DeltaSenconds);
		FSimpleHttpManage::Get()->Tick(DeltaSenconds);

		LastTime = Now;
	}
	FSimpleNetManage::Destroy(dbServer);
	FEngineLoop::AppExit();

	return 0;
}
