// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_Register.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_Register : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* NiceName;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Email;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PlayerAccount;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PlayerPassword;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ConfirmPassword;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* AuthorURL;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NiceNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EmailText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerAccountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerPasswordText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConfirmPasswordText;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

public:
	void RegisterIn();
	void RegisterOut();

protected:
	UFUNCTION()
	void Submit();

	UFUNCTION()
	void Cancel();

	void ResetText();
	void ResetColor();
};
