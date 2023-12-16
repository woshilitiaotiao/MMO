// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "UI_HallMain.generated.h"

class UUI_Print;
class UUI_CharacterCreatePanel;
class UUI_RenameCreate;
class UUI_EditorCharacter;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_HallMain : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;

	UPROPERTY(meta = (BindWidget))
	UUI_CharacterCreatePanel* UI_CharacterCreatePanel;

	UPROPERTY(meta = (BindWidget))
	UUI_RenameCreate* UI_RenameCreate;

	UPROPERTY(meta = (BindWidget))
	UUI_EditorCharacter* UI_EditorCharacter;
public:
	enum ECAType
	{
		CA_EDITOR,
		CA_CREATE,
	};
public:
	UUI_HallMain(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeDestruct();
public:
	void ResetEidtorType();
	void ResetCreateType();

	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

	void PlayRenameIn();
	void PlayRenameOut();

	void ResetCharacterCreatePanel(bool bSpawnNewCharacter = true);
	void ResetCharacterAppearance(FMMOARPGCharacterAppearance* InCA);
	void SpawnRecentCharacter();
	void HighlightDefaultSelection();
	void CheckRename(FString &InCharacterName);
	void CreateCharacter(const FMMOARPGCharacterAppearance &InCA);
	void SetSlotPosition(const int32 InSlotPosition);

	void EditCharacter(int32 InSlot);
	void DeleteCharacter(int32 InSlot);
	void SetEditCharacter(const FMMOARPGCharacterAppearance* InCA);

	void JumpDSServer(int32 InSlotID);

	void DestroyCharacter();

protected:
	void HallMainIn();
	void HallMainOut();

protected:

	void BindClientRcv();

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	void PrintLogByCheckName(ECheckNameType InCheckNameType);
private:
	FDelegateHandle RecvDelegate;
	FMMOARPGCharacterAppearance StartACData;

	//当前模式（编辑/创建）
	ECAType CAType;
};
