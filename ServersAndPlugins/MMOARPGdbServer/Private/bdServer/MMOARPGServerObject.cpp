#include "MMOARPGServerObject.h"
#include "Blueprint/SimpleMysqlObject.h"
#include "SimpleMySQLibrary.h"
#include "MyqlConfig.h"
#include "log/MMOARPGdbServerLog.h"
#include "Protocol/LoginProtocol.h"
#include "Protocol/HallProtocol.h"
#include "Protocol/ServerProtocol.h"
#include "MMOARPGType.h"
#include "Global/SimpleNetGlobalInfo.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

bool UMMOARPGServerObject::Post(const FString& InSQL)
{
	if (!InSQL.IsEmpty())
	{
		if (MysqlObjectWrite)
		{
			FString ErrMsg;
			USimpleMySQLLibrary::QueryLink(MysqlObjectWrite, InSQL, ErrMsg);

			if (ErrMsg.IsEmpty())
			{
				return true;
			}
			else
			{
				//
				UE_LOG(LogMMOARPGdbServer, Error, TEXT("MMOARPGdbServer Error: Post msg [ %s]"), *ErrMsg);
			}
		}
	}

	return false;
}

bool UMMOARPGServerObject::Get(const FString& InSQL, TArray<FSimpleMysqlResult>& Results)
{
	if (!InSQL.IsEmpty())
	{
		if (MysqlObjectRead)
		{
			FSimpleMysqlDebugResult Debug;
			Debug.bPrintToLog = true;

			FString ErrMsg;
			USimpleMySQLLibrary::QueryLinkResult(
				MysqlObjectRead,
				InSQL, 
				Results,
				ErrMsg, 
				EMysqlQuerySaveType::STORE_RESULT,
				Debug);

			if (ErrMsg.IsEmpty())
			{
				return true;
			}
			else
			{
				//
				UE_LOG(LogMMOARPGdbServer, Error, TEXT("MMOARPGdbServer Error: Get msg [ %s]"), *ErrMsg);
			}
		}
	}

	return false;
}

void UMMOARPGServerObject::Init()
{
	Super::Init();

	MysqlObjectRead = USimpleMySQLLibrary::CreateMysqlObject(NULL,
		FSimpleMysqlConfig::Get()->GetInfo().User,
		FSimpleMysqlConfig::Get()->GetInfo().Host,
		FSimpleMysqlConfig::Get()->GetInfo().Pawd,
		FSimpleMysqlConfig::Get()->GetInfo().DB,
		FSimpleMysqlConfig::Get()->GetInfo().Port,
		FSimpleMysqlConfig::Get()->GetInfo().ClientFlags);

	MysqlObjectWrite = USimpleMySQLLibrary::CreateMysqlObject(NULL,
		FSimpleMysqlConfig::Get()->GetInfo().User,
		FSimpleMysqlConfig::Get()->GetInfo().Host,
		FSimpleMysqlConfig::Get()->GetInfo().Pawd,
		FSimpleMysqlConfig::Get()->GetInfo().DB,
		FSimpleMysqlConfig::Get()->GetInfo().Port,
		FSimpleMysqlConfig::Get()->GetInfo().ClientFlags);

	//测试我们的Mysql架构
	//FString SQL = "SELECT * FROM wp_users WHERE ID = 1";
	//TArray<FSimpleMysqlResult> Results;
	//Get(SQL, Results);

	//for (auto &Tmp : Results)
	//{

	//}
	FString Create_mmoarpg_character_ca_SQL =
		TEXT("CREATE TABLE IF NOT EXISTS `mmoarpg_characters_ca`(\
		`id` INT UNSIGNED AUTO_INCREMENT,\
		`mmoarpg_name` VARCHAR(100) NOT NULL,\
		`mmoarpg_date` VARCHAR(100) NOT NULL,\
		`mmoarpg_slot` INT,\
		`Leg_Size` double(11,4) DEFAULT '0.00',\
		`Waist_Size` double(11,4) DEFAULT '0.00',\
		`Arm_Size` double(11,4) DEFAULT '0.00',\
		 PRIMARY KEY(`id`)\
		) ENGINE = INNODB DEFAULT CHARSET = utf8; ");

	if (!Post(Create_mmoarpg_character_ca_SQL))
	{
		UE_LOG(LogMMOARPGdbServer, Error, TEXT("create table mmoarpg_characters_ca failed."));
	}
}

void UMMOARPGServerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGServerObject::Close()
{
	Super::Close();

	if (MysqlObjectWrite)
	{
		MysqlObjectWrite->ConditionalBeginDestroy();
		MysqlObjectWrite = nullptr;
	}

	if (MysqlObjectRead)
	{
		MysqlObjectRead->ConditionalBeginDestroy();
		MysqlObjectRead = nullptr;
	}
}

void UMMOARPGServerObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_LoginRequests:
		{
			FString AccountString;
			FString PasswordString;
			FSimpleAddrInfo AddrInfo;

			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginRequests, AccountString, PasswordString, AddrInfo)

			FString String = TEXT("[]");

			FString SQL = FString::Printf(TEXT("SELECT ID,user_pass FROM wp_users WHERE user_login ='%s' or user_email='%s';"),
				*AccountString,
				*AccountString);

			TArray<FSimpleMysqlResult> Result;
			if (Get(SQL, Result))
			{
				if (Result.Num() > 0)
				{
					for (auto& Tmp : Result)
					{
						int32 UserID = 0;
						if (FString *IDString = Tmp.Rows.Find(TEXT("ID")))
						{
							UserID = FCString::Atoi(**IDString);
						}
						if (FString* UserPass = Tmp.Rows.Find(TEXT("user_pass")))
						{
							FString Parma = FString::Printf(TEXT("EncryptedPassword=%s&Password=%s&IP=%i&Port=%i&Channel=%s&UserID=%i"),
								**UserPass,
								*PasswordString,
								AddrInfo.Addr.IP,
								AddrInfo.Addr.Port,
								*AddrInfo.ChannelID.ToString(), 
								UserID);

							FSimpleHTTPResponseDelegate Delegate;
							Delegate.SimpleCompleteDelegate.BindUObject(this, &UMMOARPGServerObject::CheckPasswordResult);

							FString WpIP = FSimpleNetGlobalInfo::Get()->GetInfo().PublicIP;

							SIMPLE_HTTP.PostRequest(
								*FString::Printf(TEXT("http://%s/wp/wp-content/plugins/SimplePasswordVerification/SimplePasswordVerification.php"),
								*WpIP),
								*Parma,
								Delegate);
						}
					}
				}
				else
				{
					//
					ELoginType Type = ELoginType::LOGIN_ACCOUNT_WRONG;
					SIMPLE_PROTOCOLS_SEND(SP_LoginResponses, AddrInfo, Type, String);
				}
			}
			else
			{
				ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
				SIMPLE_PROTOCOLS_SEND(SP_LoginResponses, AddrInfo, Type, String);
			}

			UE_LOG(LogMMOARPGdbServer, Display, TEXT("AccountString=%s,PasswordString=%s"), *AccountString,*PasswordString);
			break;
		}
		case SP_RegisterRequests:
		{
			FString RegisterString;
			FSimpleAddrInfo AddrInfo;

			SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterRequests, RegisterString, AddrInfo)

				ERegistrationType RegistrationType = ERegistrationType::SERVER_BUG_WRONG;
			if (!RegisterString.IsEmpty())
			{
				//名字验证
				TArray<FString> Value;
				RegisterString.ParseIntoArray(Value, TEXT("&"));
				if (Value.IsValidIndex(1) && Value.IsValidIndex(2))
				{
					FString AccountString = Value[1];
					FString EmailString = Value[2];
					if (AccountString.RemoveFromStart(TEXT("Email=")) && EmailString.RemoveFromStart(TEXT("Account=")))
					{
						//账户在服务器是不是有重复的
						FString SQL = FString::Printf(TEXT("SELECT ID FROM wp_users WHERE user_login = \"%s\" or user_email= \"%s\";"),
							*AccountString,
							*EmailString);

						TArray<FSimpleMysqlResult> Result;
						if (Get(SQL, Result))
						{
							if (Result.Num() > 0)
							{
								RegistrationType = ERegistrationType::ACCOUNT_AND_EMAIL_REPETITION_ERROR;
								SIMPLE_PROTOCOLS_SEND(SP_RegisterResponses, AddrInfo, RegistrationType);
							}
							else
							{
								FString Parma = FString::Printf(TEXT("%s&IP=%i&Port=%i&Channel=%s&UserID=%i"),
									*RegisterString,
									AddrInfo.Addr.IP,
									AddrInfo.Addr.Port,
									*AddrInfo.ChannelID.ToString());

								FSimpleHTTPResponseDelegate Delegate;
								Delegate.SimpleCompleteDelegate.BindUObject(this, &UMMOARPGServerObject::CheckRegisterResult);

								FString WpIP = FSimpleNetGlobalInfo::Get()->GetInfo().PublicIP;

								SIMPLE_HTTP.PostRequest(
									*FString::Printf(TEXT("http://%s/wp/wp-content/plugins/SimpleUserRegistration/SimpleUserRegistration.php"),
										*WpIP),
									*Parma,
									Delegate);

							}
						}
					}
				}
			}
		}
		case SP_CharacterAppearanceRequests:
		{
			int32 UserID = INDEX_NONE;
			FSimpleAddrInfo AddrInfo;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceRequests, UserID, AddrInfo);
			if (UserID > 0)
			{
				//先拿到元数据
				FString IDs;

				//先拿到元数据
				TArray<FString> InIDs;
				if (GetCharacterCAInfoByUserMate(UserID, InIDs))
				{
					GetSerialString(TEXT(","), InIDs, IDs);
				}

				FCharacterAppearances CharacterAppearances;

				//拿到我们角色数据
				if (!IDs.IsEmpty())
				{
					FString SQL = FString::Printf(TEXT("SELECT * from mmoarpg_characters_ca where id in (%s);"), *IDs);
					TArray<FSimpleMysqlResult> Result;
					if (Get(SQL, Result))
					{
						if (Result.Num() > 0)
						{
							for (auto& Tmp : Result)
							{
								CharacterAppearances.Add(FMMOARPGCharacterAppearance());
								FMMOARPGCharacterAppearance& InLast = CharacterAppearances.Last();

								if (FString* InName = Tmp.Rows.Find(TEXT("mmoarpg_name")))
								{
									InLast.Name = *InName;
								}
								if (FString* InDate = Tmp.Rows.Find(TEXT("mmoarpg_date")))
								{
									InLast.Date = *InDate;
								}
								if (FString* InSlot = Tmp.Rows.Find(TEXT("mmoarpg_slot")))
								{
									InLast.SlotPosition = FCString::Atoi(**InSlot);
								}
								if (FString* InLegSize = Tmp.Rows.Find(TEXT("leg_Size")))
								{
									InLast.LegSize = FCString::Atof(**InLegSize);
								}
								if (FString* InWaistSize = Tmp.Rows.Find(TEXT("waist_size")))
								{
									InLast.WaistSize = FCString::Atof(**InWaistSize);
								}
								if (FString* InArmSize = Tmp.Rows.Find(TEXT("arm_size")))
								{
									InLast.ArmSize = FCString::Atof(**InArmSize);
								}
							}
						}
					}
				}

				FString JsonString;
				NetDataAnalysis::CharacterAppearancesToString(CharacterAppearances, JsonString);
				///
				SIMPLE_PROTOCOLS_SEND(SP_CharacterAppearanceResponses, AddrInfo, JsonString);
				UE_LOG(LogMMOARPGdbServer, Display, TEXT("SP_CharacterAppearanceResponses"));
			}
			break;
		}
		case SP_CheckCharacterNameRequests:
		{
			int32 UserID = INDEX_NONE;
			FString CharacterName;
			FSimpleAddrInfo AddrInfo;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameRequests, UserID, CharacterName, AddrInfo);

			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
			if (UserID > 0)
			{
				CheckNameType = CheckName(CharacterName);
			}

			SIMPLE_PROTOCOLS_SEND(SP_CheckCharacterNameResponses, CheckNameType, AddrInfo);

			UE_LOG(LogMMOARPGdbServer, Display, TEXT("SP_CheckCharacterNameRequests"));
			break;
		}
		case SP_CreateCharacterRequests:
		{
			int32 UserID = INDEX_NONE;
			FSimpleAddrInfo AddrInfo;
			FString JsonString;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterRequests, UserID, JsonString, AddrInfo);
			if (UserID > 0)
			{
				FMMOARPGCharacterAppearance CA;
				NetDataAnalysis::StringToCharacterAppearances(JsonString, CA);

				if (CA.SlotPosition != INDEX_NONE)
				{
					//验证名字
					ECheckNameType NameType = CheckName(CA.Name);
					bool bCreateCharacter = false;

					if (NameType == ECheckNameType::NAME_NOT_EXIST)
					{
						bool bMeta = false;

						//先拿到用户数据
						TArray<FString> CAIDs;
						{
							FString SQL = FString::Printf(TEXT("SELECT meta_value FROM wp_usermeta WHERE user_id=%i and meta_key=\"character_ca\";"), UserID);
							TArray<FSimpleMysqlResult> Result;
							if (Get(SQL, Result))
							{
								if (Result.Num() > 0)
								{
									for (auto& Tmp : Result)
									{
										if (FString* InMetaValue = Tmp.Rows.Find(TEXT("meta_value")))
										{
											InMetaValue->ParseIntoArray(CAIDs, TEXT("|"));
										}
									}

									bMeta = true;
								}

								bCreateCharacter = true;
							}
						}

						//插入数据
						if (bCreateCharacter)
						{
							FString SQL = FString::Printf(
								TEXT("INSERT INTO mmoarpg_characters_ca(\
							mmoarpg_name,mmoarpg_date,mmoarpg_slot,leg_size,waist_size,arm_size) \
							VALUES(\"%s\",\"%s\",%i,%.2lf,%.2lf,%.2lf);"),
								*CA.Name, *CA.Date, CA.SlotPosition,
								CA.LegSize, CA.WaistSize, CA.ArmSize);

							if (Post(SQL))
							{
								SQL = FString::Printf(TEXT("SELECT id FROM mmoarpg_characters_ca WHERE mmoarpg_name=\"%s\";"), *CA.Name);
								TArray<FSimpleMysqlResult> Result;
								if (Get(SQL, Result))
								{
									if (Result.Num() > 0)
									{
										for (auto& Tmp : Result)
										{
											if (FString* InIDString = Tmp.Rows.Find(TEXT("id")))
											{
												CAIDs.Add(*InIDString);
											}
										}
									}
								}
								else
								{
									bCreateCharacter = false;
								}
							}
							else
							{
								bCreateCharacter = false;
							}
						}

						//更新元数据
						if (bCreateCharacter)
						{
							FString IDStirng;
							for (auto& Tmp : CAIDs) { IDStirng += Tmp + TEXT("|"); }
							IDStirng.RemoveFromEnd(TEXT("|"));

							FString SQL;
							if (bMeta)
							{
								SQL = FString::Printf(TEXT("UPDATE wp_usermeta \
							SET meta_value=\"%s\" WHERE meta_key=\"character_ca\" and user_id=%i;"),
									*IDStirng, UserID);
							}
							else
							{
								SQL = FString::Printf(TEXT("INSERT INTO wp_usermeta( \
							user_id,meta_key,meta_value) VALUES(%i,\"character_ca\",\"%s\")"),
									UserID, *IDStirng);
							}

							if (!Post(SQL))
							{
								bCreateCharacter = false;
							}
						}
					}

					///
					SIMPLE_PROTOCOLS_SEND(SP_CreateCharacterResponses, NameType, bCreateCharacter, JsonString, AddrInfo);
					UE_LOG(LogMMOARPGdbServer, Display, TEXT("SP_CreateCharacterRequests"));
				}
			}
			break;
		}
		case SP_DeleteCharacterRequests:
		{
			int32 UserID = INDEX_NONE;
			FSimpleAddrInfo AddrInfo;
			int32 SlotID = INDEX_NONE;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterRequests, UserID, SlotID, AddrInfo);
			if (UserID != INDEX_NONE && SlotID != INDEX_NONE)
			{
				//I 获取元数据
				FString SQL = FString::Printf(TEXT("SELECT meta_value FROM wp_usermeta WHERE meta_key=\"character_ca\" and User_id=%i;"), UserID);
				TArray<FSimpleMysqlResult> Result;
				FString IDs;
				if (Get(SQL, Result))
				{
					if (Result.Num() > 0)
					{
						for (auto& Tmp : Result)
						{
							if (FString* InMetaValue = Tmp.Rows.Find(TEXT("meta_value")))
							{
								IDs = InMetaValue->Replace(TEXT("|"), TEXT(","));
							}
						}
					}
				}

				//II 获取准备移除的数据ID
				int32 SuccessDeleteCount = 0;

				SQL = FString::Printf(TEXT("SELECT id FROM mmoarpg_characters_ca WHERE id in (%s) and mmoarpg_slot=%i;"),
					*IDs, SlotID);

				FString RemoveID;
				Result.Empty();
				if (Get(SQL, Result))
				{
					if (Result.Num() > 0)
					{
						for (auto& Tmp : Result)
						{
							if (FString* InIDValue = Tmp.Rows.Find(TEXT("id")))
							{
								RemoveID = *InIDValue;
								SuccessDeleteCount++;
							}
						}
					}
				}

				//III 删除Slot对应的对象
				if (!IDs.IsEmpty())
				{
					SQL = FString::Printf(TEXT("DELETE FROM mmoarpg_characters_ca WHERE ID in (%s) and mmoarpg_slot=%i;"), *IDs, SlotID);
					if (Post(SQL))
					{
						SuccessDeleteCount++;
					}
				}

				//V 更新元数据表
				if (SuccessDeleteCount > 1)
				{
					TArray<FString> IDArray;
					IDs.ParseIntoArray(IDArray, TEXT(","));

					//移除ID
					IDArray.Remove(RemoveID);

					IDs.Empty();
					for (auto& Tmp : IDArray)
					{
						IDs += (Tmp + TEXT("|"));
					}
					IDs.RemoveFromEnd("|");

					SQL = FString::Printf(TEXT("UPDATE wp_usermeta \
							SET meta_value=\"%s\" WHERE meta_key=\"character_ca\" and user_id=%i;"),
						*IDs, UserID);

					if (Post(SQL))
					{
						SuccessDeleteCount++;
					}
				}

				SIMPLE_PROTOCOLS_SEND(SP_DeleteCharacterResponses, UserID, SlotID, SuccessDeleteCount, AddrInfo);
			}
			break;
		}
		case SP_EditorCharacterRequests:
		{
			int32 UserID = INDEX_NONE;
			FSimpleAddrInfo AddrInfo;
			FString JsonString;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterRequests, UserID, JsonString, AddrInfo);
			if (UserID > 0 && JsonString.Len() > 0)
			{
				FMMOARPGCharacterAppearance CA;
				NetDataAnalysis::StringToCharacterAppearances(JsonString, CA);

				//I 获取元数据
				FString SQL = FString::Printf(TEXT("SELECT meta_value FROM wp_usermeta WHERE meta_key=\"character_ca\" and User_id=%i;"), UserID);
				TArray<FSimpleMysqlResult> Result;
				FString IDs;
				if (Get(SQL, Result))
				{
					if (Result.Num() > 0)
					{
						for (auto& Tmp : Result)
						{
							if (FString* InMetaValue = Tmp.Rows.Find(TEXT("meta_value")))
							{
								IDs = InMetaValue->Replace(TEXT("|"), TEXT(","));
							}
						}
					}
				}

				SQL = FString::Printf(TEXT("SELECT id FROM mmoarpg_characters_ca WHERE mmoarpg_slot=%i and id in (%s)"), CA.SlotPosition, *IDs);
				Result.Empty();
				int32 UpdateID = INDEX_NONE;
				if (Get(SQL, Result))
				{
					if (Result.Num() > 0)
					{
						for (auto& Tmp : Result)
						{
							if (FString* InidValue = Tmp.Rows.Find(TEXT("id")))
							{
								UpdateID = FCString::Atoi(**InidValue);
							}
						}
					}
				}

				if (UpdateID != INDEX_NONE)
				{
					SQL = FString::Printf(
						TEXT("UPDATE mmoarpg_characters_ca \
							SET mmoarpg_name=\"%s\" ,mmoarpg_date=\"%s\",mmoarpg_slot=%i,\
							leg_size=%.2lf,waist_size=%.2lf,arm_size=%.2lf WHERE id=%i"),
						*CA.Name, *CA.Date, CA.SlotPosition,
						CA.LegSize, CA.WaistSize, CA.ArmSize, UpdateID);

					bool bUpdateSucceeded = false;
					if (Post(SQL))
					{
						bUpdateSucceeded = true;
						SIMPLE_PROTOCOLS_SEND(SP_EditorCharacterResponses, bUpdateSucceeded, AddrInfo);
						UE_LOG(LogMMOARPGdbServer, Display, TEXT("SP_EditorCharacterResponses true"));
					}
					else
					{
						SIMPLE_PROTOCOLS_SEND(SP_EditorCharacterResponses, bUpdateSucceeded, AddrInfo);
						UE_LOG(LogMMOARPGdbServer, Display, TEXT("SP_EditorCharacterResponses false"));
					}
				}
			}
			break;
		}
		case SP_PlayerRegistInfoRequests:
		{
			int32 UserID = INDEX_NONE;
			int32 SlotID = INDEX_NONE;
			FSimpleAddrInfo GateAddrInfo;
			FSimpleAddrInfo CenterAddrInfo;
			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_PlayerRegistInfoRequests, UserID, SlotID, GateAddrInfo, CenterAddrInfo);

			if (UserID != INDEX_NONE && SlotID != INDEX_NONE)
			{
				FString UserInfoJson;
				FString SlotInfoJson;
				if (GetUserInfo(UserID, UserInfoJson) && GetSlotCAInfo(UserID, SlotID, SlotInfoJson))
				{
					SIMPLE_PROTOCOLS_SEND(SP_PlayerRegistInfoResponses, UserInfoJson, SlotInfoJson, GateAddrInfo, CenterAddrInfo);
				}
				else
				{
					UserInfoJson = TEXT("[]");
					SlotInfoJson = TEXT("[]");
					SIMPLE_PROTOCOLS_SEND(SP_PlayerRegistInfoResponses, UserInfoJson, SlotInfoJson, GateAddrInfo, CenterAddrInfo);
				}
			}
		}
	}
}

ECheckNameType UMMOARPGServerObject::CheckName(const FString& InName)
{
	ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
	if (!InName.IsEmpty())
	{
		FString SQL = FString::Printf(TEXT("SELECT id FROM mmoarpg_characters_ca WHERE mmoarpg_name = \"%s\";"), *InName);

		TArray<FSimpleMysqlResult> Result;
		if (Get(SQL, Result))
		{
			if (Result.Num() > 0)
			{
				CheckNameType = ECheckNameType::NAME_EXIST;
			}
			else
			{
				CheckNameType = ECheckNameType::NAME_NOT_EXIST;
			}
		}
		else
		{
			CheckNameType = ECheckNameType::SERVER_NOT_EXIST;
		}
	}

	return CheckNameType;
}

void UMMOARPGServerObject::GetSerialString(TCHAR* InPrefix, const TArray<FString>& InStrings, FString& OutString)
{
	for (auto& Tmp : InStrings)
	{
		OutString += Tmp + InPrefix;
	}

	OutString.RemoveFromEnd(TEXT(","));
}

bool UMMOARPGServerObject::GetUserInfo(int32 InUserID, FString& OutJsonString)
{
	FMMOARPGUserData UserData;
	UserData.ID = InUserID;

	FString SQL = FString::Printf(TEXT("SELECT user_login,user_email,user_url,display_name FROM wp_users WHERE ID=%i;"), InUserID);

	TArray<FSimpleMysqlResult> Result;
	if (Get(SQL, Result))
	{
		if (Result.Num() > 0)
		{
			for (auto& Tmp : Result)
			{
				if (FString* InUserLogin = Tmp.Rows.Find(TEXT("user_login")))
				{
					UserData.Account = *InUserLogin;
				}
				if (FString* InUserEmail = Tmp.Rows.Find(TEXT("user_email")))
				{
					UserData.EMail = *InUserEmail;
				}
				//if (FString* InUserUrl = Tmp.Rows.Find(TEXT("user_url")))
				//{
				//	UserData.EMail = *InUserEmail;
				//}
				if (FString* InDisplayName = Tmp.Rows.Find(TEXT("display_name")))
				{
					UserData.Name = *InDisplayName;
				}
			}
		}
	}

	//解析用户数据
	NetDataAnalysis::UserDataToString(UserData, OutJsonString);

	return OutJsonString.Len() > 0;
}

bool UMMOARPGServerObject::GetSlotCAInfo(int32 InUserID, int32 InSlotCAID, FString& OutJsonString)
{
	TArray<FString> IDs;
	if (GetCharacterCAInfoByUserMate(InUserID, IDs))
	{
		FString IDString;
		GetSerialString(TEXT(","), IDs, IDString);

		FString SQL = FString::Printf(TEXT("SELECT * from mmoarpg_characters_ca where id in(%s) and mmoarpg_slot=%i;"), *IDString, InSlotCAID);
		TArray<FSimpleMysqlResult> Result;
		if (Get(SQL, Result))
		{
			if (Result.Num() > 0)
			{
				FMMOARPGCharacterAppearance CA;
				for (auto& Tmp : Result)
				{
					if (FString* InName = Tmp.Rows.Find(TEXT("mmoarpg_name")))
					{
						CA.Name = *InName;
					}
					if (FString* InDate = Tmp.Rows.Find(TEXT("mmoarpg_date")))
					{
						CA.Date = *InDate;
					}
					if (FString* InSlot = Tmp.Rows.Find(TEXT("mmoarpg_slot")))
					{
						CA.SlotPosition = FCString::Atoi(**InSlot);
					}
					if (FString* InLegSize = Tmp.Rows.Find(TEXT("leg_Size")))
					{
						CA.LegSize = FCString::Atof(**InLegSize);
					}
					if (FString* InWaistSize = Tmp.Rows.Find(TEXT("waist_size")))
					{
						CA.WaistSize = FCString::Atof(**InWaistSize);
					}
					if (FString* InArmSize = Tmp.Rows.Find(TEXT("arm_size")))
					{
						CA.ArmSize = FCString::Atof(**InArmSize);
					}
				}

				NetDataAnalysis::CharacterAppearancesToString(CA, OutJsonString);

				return !OutJsonString.IsEmpty();
			}
		}
	}

	return false;
}

bool UMMOARPGServerObject::GetCharacterCAInfoByUserMate(int32 InUserID, TArray<FString>& OutIDs)
{
	//先拿到元数据
	FString SQL = FString::Printf(TEXT("SELECT meta_value from wp_usermeta where user_id=%i and meta_key=\"character_ca\";"), InUserID);
	TArray<FSimpleMysqlResult> Result;
	if (Get(SQL, Result))
	{
		if (Result.Num() > 0)
		{
			for (auto& Tmp : Result)
			{
				if (FString* InMetaValue = Tmp.Rows.Find(TEXT("meta_value")))
				{
					InMetaValue->ParseIntoArray(OutIDs, TEXT("|"));
				}
			}
		}

		return true;
	}

	return OutIDs.Num() > 0;
}

void UMMOARPGServerObject::CheckPasswordResult(const FSimpleHttpRequest& InRequest, const FSimpleHttpResponse& InResponse, bool bLinkSuccessful)
{
	if (bLinkSuccessful)
	{
		//xx&IP&Port&0
		TArray<FString> Values;
		InResponse.ResponseMessage.ParseIntoArray(Values,TEXT("&"));

		FSimpleAddrInfo AddrInfo;
		uint32 UserID = 0;
		EPasswordVerification PV = EPasswordVerification::VERIFICATION_FAIL;
		if (Values.Num())
		{
			if (Values.IsValidIndex(0))
			{
				UserID = FCString::Atoi(*Values[0]);
			}
			if (Values.IsValidIndex(1))
			{
				AddrInfo.Addr.IP = FCString::Atoi(*Values[1]);
			}
			if (Values.IsValidIndex(2))
			{
				AddrInfo.Addr.Port = FCString::Atoi(*Values[2]);
			}
			if (Values.IsValidIndex(3))
			{
				FGuid::ParseExact(Values[3], EGuidFormats::Digits, AddrInfo.ChannelID);
			}
			if (Values.IsValidIndex(4))
			{
				PV = (EPasswordVerification)FCString::Atoi(*Values[4]);
			}

			FString String = TEXT("[]");

			if (PV == VERIFICATION_SUCCESS)
			{
				if (UserID != 0)
				{
					//FMMOARPGUserData UserData;
					//UserData.ID = UserID;

					//FString SQL = FString::Printf(TEXT("SELECT user_login,user_email,user_url,display_name FROM wp_users WHERE ID=%i;"), UserID);
					//
					//TArray<FSimpleMysqlResult> Result;
					//if (Get(SQL, Result))
					//{
					//	if (Result.Num() > 0)
					//	{
					//		for (auto& Tmp : Result)
					//		{
					//			if (FString* InUserLogin = Tmp.Rows.Find(TEXT("user_login")))
					//			{
					//				UserData.Account = *InUserLogin;
					//			}
					//			if (FString* InUserEmail = Tmp.Rows.Find(TEXT("user_email")))
					//			{
					//				UserData.EMail = *InUserEmail;
					//			}
					//			//if (FString* InUserUrl = Tmp.Rows.Find(TEXT("user_url")))
					//			//{
					//			//	UserData.EMail = *InUserEmail;
					//			//}
					//			if (FString* InDisplayName = Tmp.Rows.Find(TEXT("display_name")))
					//			{
					//				UserData.Name = *InDisplayName;
					//			}
					//		}
					//	}
					//}
					//
					////解析用户数据
					//NetDataAnalysis::UserDataToString(UserData, String);

					if (GetUserInfo(UserID, String))
					{
						ELoginType Type = ELoginType::LOGIN_SUCCESS;
						SIMPLE_PROTOCOLS_SEND(SP_LoginResponses, AddrInfo, Type, String);
					}

				}
			}
			else
			{
				ELoginType Type = ELoginType::LOGIN_WRONG_PASSWORD;
				SIMPLE_PROTOCOLS_SEND(SP_LoginResponses, AddrInfo, Type, String);
			}
		}
	}
}

void UMMOARPGServerObject::CheckRegisterResult(const FSimpleHttpRequest& InRequest, const FSimpleHttpResponse& InResponse, bool bLinkSuccessful)
{
	//2130706433&51509&3B76DAFE4F31856008E3AB82AED291C3&0
	if (bLinkSuccessful)
	{
		//xx&IP&Port&0
		TArray<FString> Values;
		InResponse.ResponseMessage.ParseIntoArray(Values, TEXT("&"));

		FSimpleAddrInfo AddrInfo;
		uint32 UserID = 0;
		ERegistrationVerification RV = ERegistrationVerification::REGISTRATION_FAIL;
		if (Values.Num())
		{
			if (Values.IsValidIndex(0))
			{
				AddrInfo.Addr.IP = FCString::Atoi(*Values[0]);
			}
			if (Values.IsValidIndex(1))
			{
				AddrInfo.Addr.Port = FCString::Atoi(*Values[1]);
			}
			if (Values.IsValidIndex(2))
			{
				FGuid::ParseExact(Values[2], EGuidFormats::Digits, AddrInfo.ChannelID);
			}
			if (Values.IsValidIndex(3))
			{
				RV = (ERegistrationVerification)FCString::Atoi(*Values[3]);
			}

			ERegistrationType RegistrationType = ERegistrationType::SERVER_BUG_WRONG;
			if (RV == REGISTRATION_SUCCESS)
			{
				RegistrationType = ERegistrationType::PLAYER_REGISTRATION_SUCCESS;
				SIMPLE_PROTOCOLS_SEND(SP_RegisterResponses, AddrInfo, RegistrationType);
			}
			else
			{
				SIMPLE_PROTOCOLS_SEND(SP_RegisterResponses, AddrInfo, RegistrationType);
			}
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif