// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_Login.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_Login : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Account;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Password;

	UPROPERTY(meta = (BindWidget))
	UButton* SignInButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RegisterButton;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	bool EncryptionToLocal(const FString& InPaths);
	bool DecryptionFromLocal(const FString& InPaths);
protected:
	UFUNCTION()
	void SignIn();

	UFUNCTION()
	void Register();

	UFUNCTION()
	void CallDropDownBox();
private:
	TMap<FString, FString> OutMap;
};
