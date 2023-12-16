// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_LoginMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "UI_Login.h"
#include "Protocol/LoginProtocol.h"
#include "../../MMOARPGMacroType.h"
#include "../Common/UI_Print.h"
#include "Kismet/GameplayStatics.h"
#include "UI_Register.h"
#include "MMOARPGType.h"
#define LOCTEXT_NAMESPACE "UUI_LoginMain"

void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();

	PlayWidgetAnim(TEXT("LoginIn"));

	UI_Login->SetParents(this);
	
	UI_Register->SetParents(this);

	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_LoginMain::LinkServerInfo);

			InGameInstance->LinkServer();

			BindClientRcv();
		}	
	}

	//读取账号
	if (!UI_Login->DecryptionFromLocal(FPaths::ProjectDir() / TEXT("User")))
	{
		PrintLog(TEXT("No account detected."));
	}	
}

void UUI_LoginMain::NativeDestruct()
{
	Super::NativeDestruct();

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(RecvDelegate);
		}
	}
}

void UUI_LoginMain::SignIn(FString& InAccount,FString& InPassword)
{
	SEND_DATA(SP_LoginRequests, InAccount, InPassword);
}

void UUI_LoginMain::Register()
{
	UI_Register->RegisterIn();
}

void UUI_LoginMain::Register(FString InRegisterInfo)
{
	SEND_DATA(SP_RegisterRequests, InRegisterInfo);
}

void UUI_LoginMain::PrintLog(const FString& InMsg)
{
	PrintLog(FText::FromString(InMsg));
}

void UUI_LoginMain::PrintLog(const FText& InMsg)
{
	//播放动画
	UI_Print->PlayTextAnim();

	UI_Print->SetText(InMsg);
}

void UUI_LoginMain::BindClientRcv()
{
	if (UMMOARPGGameInstance *InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			RecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda(
			[&](uint32 ProtocolNumber, FSimpleChannel* Channel)
			{
				this->RecvProtocol(ProtocolNumber, Channel);
			});
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
		}
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	}
}

void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
		case SP_LoginResponses:
		{
			FString String;
			ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
			FMMOARPGGateStatus GateStatus;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses, Type, String, GateStatus);

			switch (Type)
			{
			case LOGIN_DB_SERVER_ERROR:
				PrintLog(TEXT("Server error."));
				break;
			case LOGIN_SUCCESS:
			{
				if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
				{
					NetDataAnalysis::StringToUserData(String, InGameInstance->GetUserData());					
					if (GateStatus.GateConnetionNum == INDEX_NONE)
					{
						//等待提示

						PrintLog(TEXT("At present, the number of servers is full, and we will try to link again."));
					}
					else
					{
						InGameInstance->GetGateStatus() = GateStatus;

						if (!UI_Login->EncryptionToLocal(FPaths::ProjectDir() / TEXT("User")))
						{
							PrintLog(TEXT("Password storage failed."));
						}
						else
						{
							PrintLog(TEXT("LOGIN SUCCESS."));
						}

						PlayWidgetAnim(TEXT("LoginOut"));

						//要关闭我们的 Login服务器
						if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetChannel())
						{
							InGameInstance->GetClient()->GetChannel()->DestroySelf();
						}

						//协程
						GThread::Get()->GetCoroutines().BindLambda(2.f, [&]() 
						{
							UGameplayStatics::OpenLevel(GetWorld(),TEXT("HallMap"));
						});
					}
				}

				break;
			}
			case LOGIN_ACCOUNT_WRONG:
				PrintLog(TEXT("Account does not exist."));
				break;
			case LOGIN_WRONG_PASSWORD:
				PrintLog(TEXT("Password verification failed."));
				break;
			}

			break;
		}
		case SP_RegisterResponses:
		{
			ERegistrationType Type = ERegistrationType::SERVER_BUG_WRONG;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterResponses, Type);

			switch (Type)
			{
			case ACCOUNT_AND_EMAIL_REPETITION_ERROR:
			{
				PrintLog(LOCTEXT("ACCOUNT_AND_EMAIL_REPETITION_ERROR", "Duplicate account or email."));

				//协程
				GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]()
					{
						Register();
					});

				break;
			}
			case PLAYER_REGISTRATION_SUCCESS:
				PrintLog(LOCTEXT("REGISTRATION_SUCCESS", "Registration was successful."));
				break;
			case SERVER_BUG_WRONG:
				PrintLog(LOCTEXT("SERVER_BUG_WRONG", "Server unknown error."));
				break;
			default:
				break;
			}
		}
	}
}

void UUI_LoginMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}