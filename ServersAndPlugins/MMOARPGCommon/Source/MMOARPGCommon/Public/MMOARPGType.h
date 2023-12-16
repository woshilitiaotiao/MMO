#pragma once

#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.generated.h"

enum ELoginType
{
	LOGIN_DB_SERVER_ERROR,
	LOGIN_SUCCESS,
	LOGIN_ACCOUNT_WRONG,
	LOGIN_WRONG_PASSWORD,
};

enum ERegistrationType
{
	ACCOUNT_AND_EMAIL_REPETITION_ERROR,//名字重复
	PLAYER_REGISTRATION_SUCCESS,//注册成功
	SERVER_BUG_WRONG,
};

enum ECheckNameType
{
	UNKNOWN_ERROR,
	NAME_EXIST,
	NAME_NOT_EXIST,
	SERVER_NOT_EXIST,
};

struct MMOARPGCOMMON_API FMMOARPGUserData
{
	FMMOARPGUserData()
		:ID(INDEX_NONE)
	{}

	int32 ID;
	FString Account;
	FString EMail;
	FString Name;
	FString HeadPortraitURL;

	void Reset();
};

struct MMOARPGCOMMON_API FMMOARPGGateStatus
{
	int32 GateConnetionNum;
	FSimpleAddrInfo GateServerAddrInfo;
};

USTRUCT()
struct MMOARPGCOMMON_API FMMOARPGCharacterAppearance
{
	GENERATED_USTRUCT_BODY()

	FMMOARPGCharacterAppearance()
		:Lv(INDEX_NONE)
		, SlotPosition(INDEX_NONE)
		, LegSize(0.f)
		, WaistSize(0.f)
		, ArmSize(0.f)
	{}

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Date;

	UPROPERTY()
	int32 Lv;

	UPROPERTY()
	int32 SlotPosition;

	UPROPERTY()
	float LegSize;

	UPROPERTY()
	float WaistSize;

	UPROPERTY()
	float ArmSize;

	void Reset();
};

FORCEINLINE bool operator==(const FMMOARPGCharacterAppearance& Lhs, const FMMOARPGCharacterAppearance& Rhs)
{
	return Lhs.SlotPosition == Rhs.SlotPosition;
}

typedef TArray<FMMOARPGCharacterAppearance> FCharacterAppearances;

struct MMOARPGCOMMON_API FMMOARPGPlayerRegistInfo
{
	FMMOARPGUserData UserInfo;
	FMMOARPGCharacterAppearance CAInfo;

	void Reset();
	bool IsVaild();
};

namespace NetDataAnalysis
{
	void MMOARPGCOMMON_API UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString);
	void MMOARPGCOMMON_API StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData);

	void MMOARPGCOMMON_API CharacterAppearancesToString(const FCharacterAppearances &InCAs, FString &OutString);
	void MMOARPGCOMMON_API CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCAs, FString& OutString);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& OutString, FCharacterAppearances& InCAs);
	void MMOARPGCOMMON_API StringToCharacterAppearances(const FString& OutString, FMMOARPGCharacterAppearance& InCAs);
}