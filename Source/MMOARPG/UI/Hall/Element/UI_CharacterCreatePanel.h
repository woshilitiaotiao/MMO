// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMOARPGType.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterCreatePanel.generated.h"

class UTextBlock;
class UScrollBox;
class UUI_CharacterButton;
class UUI_KneadFace;
class ACharacterStage;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterCreatePanel : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PanelName;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* List;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_CharacterButton> UI_CharacterButtonClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_KneadFace> UI_KneadFaceClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<ACharacterStage> CharacterStageClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FVector SpawnPoint;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	UUI_KneadFace *CreateKneadFace();
	void CreateCharacterButtons();

	void InitCharacterButtons(const FCharacterAppearances &InCAs);

	ACharacterStage *CreateCharacter();
	void SpawnCharacter(const int32 InSlotPos);
	void SpawnCharacter(const FMMOARPGCharacterAppearance *InACData);
	void SpawnCharacter(); 

	void SetCurrentSlotPosition(const int32 InNewPos);
	void HighlightSelection(int32 InNewPos);
	
	UUI_CharacterButton* GetHighlightButton();
	void JumpDSServer(int32 InSlotID);

	//bool 代表着是否继续循环
	template<class T>
	T* FindByPredicateInList(TFunction<bool(T*)> Func)
	{
		for (auto& Tmp : List->GetAllChildren())
		{
			if (T* InButton = Cast<T>(Tmp))
			{
				if (Func(InButton))
				{
					return InButton;
				}
			}
		}
		return NULL;
	}

protected:
	int32 SlotPosition;//��¼�ŵ�ǰѡ��Ľ�ɫ
};
