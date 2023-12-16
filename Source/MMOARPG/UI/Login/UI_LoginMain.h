// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "UI_LoginMain.generated.h"

class FSimpleChannel;
class UUI_Login;
class UUI_Print;
class UUI_Register;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_LoginMain : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UUserWidget* UI_LinkWidget;

	UPROPERTY(meta = (BindWidget))
	UUI_Login* UI_Login;

	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;

	UPROPERTY(meta = (BindWidget))
	UUI_Register* UI_Register;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

public:
	void SignIn(FString& ,FString& );

	void Register();
	void Register(FString InRegisterInfo);

	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);
protected:

	void BindClientRcv();

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

private:
	FDelegateHandle RecvDelegate;
};
