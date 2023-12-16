#pragma once
#include "MMOARPGdbServerType.h"

enum EPasswordVerification
{
	VERIFICATION_SUCCESS = 0,
	VERIFICATION_FAIL,
};

enum ERegistrationVerification
{
	REGISTRATION_SUCCESS = 0,
	REGISTRATION_FAIL,
};

class FSimpleMysqlConfig
{
public:
	static FSimpleMysqlConfig* Get();
	static void Destroy();

	void Init(const FString &InPath = FPaths::ProjectDir()/TEXT("MysqlConfig.ini"));
	const FMysqlConfig& GetInfo() const;
private:
	static FSimpleMysqlConfig* Global;
	FMysqlConfig ConfigInfo;
};