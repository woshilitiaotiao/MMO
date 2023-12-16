// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI_LoginMain.h"
#include "Stream/SimpleIOStream.h"

void UUI_Login::SignIn()
{
	if (UUI_LoginMain *InLoginMain = GetParents<UUI_LoginMain>())
	{
		FString AccountString = Account->GetText().ToString();
		FString PasswordString = Password->GetText().ToString();

		InLoginMain->SignIn(AccountString, PasswordString);
	}
}

void UUI_Login::Register()
{
	if (UUI_LoginMain* InLoginMain = GetParents<UUI_LoginMain>())
	{
		InLoginMain->Register();
	}
}

void UUI_Login::CallDropDownBox()
{

}

void UUI_Login::NativeConstruct()
{
	Super::NativeConstruct();

	SignInButton->OnReleased.AddDynamic(this,&UUI_Login::SignIn);
	RegisterButton->OnReleased.AddDynamic(this, &UUI_Login::Register);
}

void UUI_Login::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UUI_Login::EncryptionToLocal(const FString& InPaths)
{
	FString AccountString = Account->GetText().ToString();
	FString PasswordString = Password->GetText().ToString();

	//����
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);//\\->/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	if (!AccountString.IsEmpty() && !PasswordString.IsEmpty() && !InPaths.IsEmpty())
	{
		TArray<uint8> PasswordBuff;
		FSimpleIOStream Stream(PasswordBuff);

		Stream << PasswordString;

		FString HashKey = FMD5::HashAnsiString(*AccountString);
		for (int32 i =0 ;i < 32; i++)
		{
			HashKey = FMD5::HashAnsiString(*HashKey);
		}

		if (!HashKey.IsEmpty())
		{
			int32 Remain = 0;
			int32 Total = PasswordBuff.Num();
			if (Total % 16)
			{
				float Value = (float)Total / 16.f;
				int32 Ceil = FMath::CeilToInt(Value);

				int32 CeilTotal = Ceil * 16;
				Remain = CeilTotal - Total;
			}

			for (int32 i = 0; i < Remain; i++)
			{
				uint8 Tmp = 0;
				Stream << Tmp;
			}

			ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
			FAES::EncryptData(PasswordBuff.GetData(), PasswordBuff.Num(), InKey);

			//ɾ��ԭ�Ⱦɰ汾���˻�������
			IFileManager::Get().DeleteDirectory(*InCypPaths);

			FFileHelper::SaveArrayToFile(PasswordBuff, *(InCypPaths / AccountString + TEXT(".arpg")));

			return true;
		}
	}

	return false;
}

bool UUI_Login::DecryptionFromLocal(const FString& InPaths)
{
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);//\\->/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	TArray<FString> Filenames;
	IFileManager::Get().FindFilesRecursive(Filenames, *InPaths, TEXT("*.*"), true, false);

	for (auto& Tmp : Filenames)
	{
		//D:\new_new_vide2\111.arpg
		FString FilenameHeard = FPaths::GetCleanFilename(Tmp);
		if (FilenameHeard.Contains(TEXT(".arpg")))
		{
			TArray<uint8> InByte;
			FFileHelper::LoadFileToArray(InByte, *Tmp);

			FilenameHeard.RemoveFromEnd(TEXT(".arpg"));

			if (InByte.Num() > 0)
			{
				FString HashKey = FMD5::HashAnsiString(*FilenameHeard);
				for (int32 i = 0; i < 32; i++)
				{
					HashKey = FMD5::HashAnsiString(*HashKey);
				}

				if (!HashKey.IsEmpty())
				{
					FSimpleIOStream Stream(InByte);

					ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
					FAES::DecryptData(InByte.GetData(), InByte.Num(), InKey);

					FString PasswordString;
					Stream >> PasswordString;

					Account->SetText(FText::FromString(FilenameHeard));
					Password->SetText(FText::FromString(PasswordString));

					return true;
				}
			}
		}
	}

	return false;
}