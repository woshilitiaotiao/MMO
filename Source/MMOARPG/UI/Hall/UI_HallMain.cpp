// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_HallMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMOARPGMacroType.h"
#include "../Common/UI_Print.h"
#include "Kismet/GameplayStatics.h"
#include "Element/UI_CharacterCreatePanel.h"
#include "Element/UI_RenameCreate.h"
#include "Protocol/HallProtocol.h"
#include "../../Core/Hall/HallPlayerState.h"
#include "Element/KneadFace/UI_EditorCharacter.h"
#include "../../Core/Hall/HallPawn.h"
#include "../../Core/Hall/Character/CharacterStage.h"
#include "Element/UI_KneadFace.h"

#define LOCTEXT_NAMESPACE "UUI_HallMain"

UUI_HallMain::UUI_HallMain(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,CAType(ECAType::CA_CREATE)
{

}

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	HallMainIn();
	UI_RenameCreate->SetParents(this);
	UI_CharacterCreatePanel->SetParents(this);
	UI_EditorCharacter->SetParents(this);

	//�����ͻ���
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_HallMain::LinkServerInfo);
			
			InGameInstance->GetClient()->Init(InGameInstance->GetGateStatus().GateServerAddrInfo.Addr);
	
			BindClientRcv();
		}
	}
}

void UUI_HallMain::NativeDestruct()
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

void UUI_HallMain::PrintLog(const FString& InMsg)
{
	PrintLog(FText::FromString(InMsg));
}

void UUI_HallMain::PrintLog(const FText& InMsg)
{
	//���Ŷ���
	UI_Print->PlayTextAnim();

	UI_Print->SetText(InMsg);
}

void UUI_HallMain::PlayRenameIn()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameIn"));
}

void UUI_HallMain::PlayRenameOut()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameOut"));
}

void UUI_HallMain::ResetCharacterCreatePanel(bool bSpawnNewCharacter)
{
	UI_CharacterCreatePanel->CreateCharacterButtons();

	if (bSpawnNewCharacter)
	{
		//��������Ķ���
		SpawnRecentCharacter();
	}

	HighlightDefaultSelection();
}

void UUI_HallMain::DestroyCharacter()
{
	//ɾ���ոս�ɫ
	if (AHallPawn* InPawn = GetPawn<AHallPawn>())
	{
		if (InPawn->CharacterStage)
		{
			InPawn->CharacterStage->Destroy();
			InPawn->CharacterStage = nullptr;
		}
	}
}

void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMOARPGCharacterAppearance *InCAData = InState->GetRecentCharacter())
		{
			UI_CharacterCreatePanel->SpawnCharacter(InCAData);

			SetEditCharacter(InCAData);
		}
		else
		{
			SetEditCharacter(NULL);	
			DestroyCharacter();
		}
	}
}

void UUI_HallMain::HighlightDefaultSelection()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMOARPGCharacterAppearance* InCAData = InState->GetRecentCharacter())
		{
			UI_CharacterCreatePanel->HighlightSelection(InCAData->SlotPosition);
		}
	}
}

void UUI_HallMain::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
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

void UUI_HallMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
		case SP_CharacterAppearanceResponses:
		{
			FString CharacterJson;

			//�õ��ͻ��˷��͵��˺�
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);
			if (!CharacterJson.IsEmpty())
			{
				if (AHallPlayerState *InState = GetPlayerState<AHallPlayerState>())
				{
					NetDataAnalysis::StringToCharacterAppearances(CharacterJson,InState->GetCharacterAppearance());
				
					UI_CharacterCreatePanel->InitCharacterButtons(InState->GetCharacterAppearance());

					//�������ʹ�ý�ɫ
					SpawnRecentCharacter();

					//�����Ǹ���
					HighlightDefaultSelection();
				}
			}

			break;
		}
		case SP_CheckCharacterNameResponses:
		{
			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
			SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameResponses, CheckNameType);

			PrintLogByCheckName(CheckNameType);

			break;
		}
		case SP_CreateCharacterResponses:
		{
			ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
			bool bCreateCharacter = false;
			FString JsonString;
			SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, CheckNameType, bCreateCharacter,JsonString);

			if (bCreateCharacter)
			{
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_SUCCESSFULLY", "created successfully."));
				
				FMMOARPGCharacterAppearance InCA;
				NetDataAnalysis::StringToCharacterAppearances(JsonString, InCA);
				if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
				{
					InPlayerState->AddCharacterCA(InCA);

					//����
					PlayRenameOut();
					ResetCharacterCreatePanel(false);

					SetEditCharacter(&InCA);
				}
			}
			else
			{
				PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_FAIL", "created fail."));
				GThread::Get()->GetCoroutines().BindLambda(1.5f, [=]() { PrintLogByCheckName(CheckNameType); });
			}
			
			break;
		}
		case SP_DeleteCharacterResponses:
		{
			int32 UserID = INDEX_NONE;
			int32 SlotID = INDEX_NONE;
			int32 SuccessDeleteCount = 0;
			SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterResponses, UserID, SlotID, SuccessDeleteCount);

			if (SuccessDeleteCount > 2)
			{
				if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
				{
					InState->RemoveCharacterAppearanceBySlot(SlotID);
					
					ResetCharacterCreatePanel(true);
				}

				PrintLog(LOCTEXT("DELETE_CHARACTER_SUCCESS", "The role deletion is successful, and the change operation is irreversible."));
			}
			else
			{
				PrintLog(LOCTEXT("DELETE_CHARACTER_ERROR", "Failed to delete the role. Please check if the role exists."));
			}
		}
		case SP_EditorCharacterResponses:
		{
			FSimpleAddrInfo AddrInfo;
			bool bUpdateSucceeded = false;

			SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterResponses, bUpdateSucceeded);

			if (bUpdateSucceeded)
			{	
				PrintLog(LOCTEXT("EDITORCHARACTERRESPONSES_SUCCESSFULLY", "Edit character succeeded."));
			}
			else
			{
				PrintLog(LOCTEXT("EDITORCHARACTERRESPONSES_ERROR", "Edit character Error."));
			}

			//����
			PlayRenameOut();
			ResetCharacterCreatePanel(false);
		}
		case SP_LoginToDSServerResponses:
		{
			FSimpleAddr Addr;
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, Addr);

			FString DSAddrString = FSimpleNetManage::GetAddrString(Addr);

			HallMainOut();

			//协程
			GThread::Get()->GetCoroutines().BindLambda(1.f, [=]()
				{
					UGameplayStatics::OpenLevel(GetWorld(), *DSAddrString);
				});
		}
	}
}

void UUI_HallMain::EditCharacter(int32 InSlot)
{
	PlayRenameIn();
	SetSlotPosition(InSlot);

	if (UUI_KneadFace* InKneadFace = UI_CharacterCreatePanel->CreateKneadFace())
	{
		if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
		{
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(InSlot))
			{
				InKneadFace->InitKneadFace(InCA);
				StartACData = *InCA;

				ResetEidtorType();
				UI_EditorCharacter->SetIsEnabled(false);

				//��������
				UI_RenameCreate->SetEditableName(FText::FromString(InCA->Name));
			}
		}
	}
}

void UUI_HallMain::PrintLogByCheckName(ECheckNameType InCheckNameType)
{
	switch (InCheckNameType)
	{
	case UNKNOWN_ERROR:
		PrintLog(LOCTEXT("CHECK_NAME_UNKNOWN_ERROR", "The server encountered an unknown error."));
		break;
	case NAME_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is valid"));
		break;
	case SERVER_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
		break;
	case NAME_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
		break;
	}
}

void UUI_HallMain::ResetCharacterAppearance(FMMOARPGCharacterAppearance* InCA)
{
	if (CAType == ECAType::CA_EDITOR)
	{
		if (InCA)
		{
			*InCA = StartACData;
		}
	}

	UI_EditorCharacter->SetIsEnabled(true);
}

void UUI_HallMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
		{	
			//���ͽ�ɫ��������
			SEND_DATA(SP_CharacterAppearanceRequests,InGameInstance->GetUserData().ID);
		}
	}
}

void UUI_HallMain::CheckRename(FString& InCharacterName)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		SEND_DATA(SP_CheckCharacterNameRequests, InGameInstance->GetUserData().ID, InCharacterName);
	}
}

void UUI_HallMain::CreateCharacter(const FMMOARPGCharacterAppearance& InCA)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		FString CAJson;
		if (CAType == ECAType::CA_EDITOR)
		{
			if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
			{
				if (FMMOARPGCharacterAppearance* InNewCA = InState->GetCharacterCA(UI_RenameCreate->GetSlotPosition()))
				{
					InNewCA->Date = InCA.Date;
					InNewCA->Name = InCA.Name;

					NetDataAnalysis::CharacterAppearancesToString(*InNewCA, CAJson);
				}
			}

			SEND_DATA(SP_EditorCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}
		else
		{
			NetDataAnalysis::CharacterAppearancesToString(InCA, CAJson);
			SEND_DATA(SP_CreateCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}
		
		UI_EditorCharacter->SetIsEnabled(true);
	}
}

void UUI_HallMain::ResetEidtorType()
{
	CAType = ECAType::CA_EDITOR;
}

void UUI_HallMain::ResetCreateType()
{
	CAType = ECAType::CA_CREATE;
}

void UUI_HallMain::SetSlotPosition(const int32 InSlotPosition)
{
	UI_RenameCreate->SetSlotPosition(InSlotPosition);
}

void UUI_HallMain::DeleteCharacter(int32 InSlot)
{
	if (InSlot >= 0 && InSlot < 4)
	{
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
		{
			//���ͽ�ɫ��������
			SEND_DATA(SP_DeleteCharacterRequests, InGameInstance->GetUserData().ID, InSlot);
		}
	}
}

void UUI_HallMain::SetEditCharacter(const FMMOARPGCharacterAppearance* InCA)
{
	if (InCA)
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(InCA->Name));
		UI_EditorCharacter->SetSlotID(InCA->SlotPosition);
	}
	else
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(""));
		UI_EditorCharacter->SetSlotID(INDEX_NONE);
	}
}

void UUI_HallMain::JumpDSServer(int32 InSlotID)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		SEND_DATA(SP_LoginToDSServerRequests, InGameInstance->GetUserData().ID, InSlotID);
	}
}

void UUI_HallMain::HallMainIn()
{
	PlayWidgetAnim(TEXT("HallMainIn"));
}

void UUI_HallMain::HallMainOut()
{
	PlayWidgetAnim(TEXT("HallMainOut"));
}

#undef LOCTEXT_NAMESPACE