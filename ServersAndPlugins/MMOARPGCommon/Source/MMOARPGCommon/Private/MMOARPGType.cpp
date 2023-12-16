#include "MMOARPGType.h"
#include "Json.h"

void FMMOARPGPlayerRegistInfo::Reset()
{
	UserInfo.Reset();
	CAInfo.Reset();
}

bool FMMOARPGPlayerRegistInfo::IsVaild()
{
	return UserInfo.ID != INDEX_NONE;
}

void FMMOARPGCharacterAppearance::Reset()
{
	Lv = INDEX_NONE;
	SlotPosition = INDEX_NONE;
	LegSize = 0.0f;
	WaistSize = 0.0f;
	ArmSize = 0.0f;

	Name.Empty();
	Date.Empty();
}

namespace NetDataAnalysis
{
	void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("ID"), InUserData.ID);
		JsonWriter->WriteValue(TEXT("Account"), InUserData.Account);
		JsonWriter->WriteValue(TEXT("EMail"), InUserData.EMail);
		JsonWriter->WriteValue(TEXT("Name"), InUserData.Name);
		JsonWriter->WriteValue(TEXT("HeadPortraitURL"), InUserData.HeadPortraitURL);

		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			OutUserData.ID = ReadRoot->GetIntegerField(TEXT("ID"));
			OutUserData.Account = ReadRoot->GetStringField(TEXT("Account"));
			OutUserData.Name = ReadRoot->GetStringField(TEXT("Name"));
			OutUserData.EMail = ReadRoot->GetStringField(TEXT("EMail"));
			OutUserData.HeadPortraitURL = ReadRoot->GetStringField(TEXT("HeadPortraitURL"));
		}
	}

	void CharacterAppearancesToString(const FCharacterAppearances& InCAs, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteArrayStart();
		for(auto &Tmp : InCAs)
		{
			JsonWriter->WriteObjectStart();

			JsonWriter->WriteValue(TEXT("Name"),Tmp.Name);
			JsonWriter->WriteValue(TEXT("Date"), Tmp.Date);
			JsonWriter->WriteValue(TEXT("Lv"), Tmp.Lv);
			JsonWriter->WriteValue(TEXT("SlotPosition"), Tmp.SlotPosition);
			JsonWriter->WriteValue(TEXT("LegSize"), Tmp.LegSize);
			JsonWriter->WriteValue(TEXT("WaistSize"), Tmp.WaistSize);
			JsonWriter->WriteValue(TEXT("ArmSize"), Tmp.ArmSize);

			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();
	}

	void CharacterAppearancesToString(const FMMOARPGCharacterAppearance& InCAs, FString& OutString)
	{
		OutString.Empty();

		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("Name"), InCAs.Name);
			JsonWriter->WriteValue(TEXT("Date"), InCAs.Date);
			JsonWriter->WriteValue(TEXT("Lv"), InCAs.Lv);
			JsonWriter->WriteValue(TEXT("SlotPosition"), InCAs.SlotPosition);
			JsonWriter->WriteValue(TEXT("LegSize"), InCAs.LegSize);
			JsonWriter->WriteValue(TEXT("WaistSize"), InCAs.WaistSize);
			JsonWriter->WriteValue(TEXT("ArmSize"), InCAs.ArmSize);
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void StringToCharacterAppearances(const FString& OutString, FCharacterAppearances& InCAs)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(OutString);
		TArray<TSharedPtr<FJsonValue>> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			for (auto &Tmp : ReadRoot)
			{
				InCAs.Add(FMMOARPGCharacterAppearance());
				FMMOARPGCharacterAppearance &InLast = InCAs.Last();
				if (TSharedPtr<FJsonObject> InJsonObject = Tmp->AsObject())
				{
					InLast.Name = InJsonObject->GetStringField(TEXT("Name"));
					InLast.Date = InJsonObject->GetStringField(TEXT("Date"));
					InLast.Lv = InJsonObject->GetIntegerField(TEXT("Lv"));
					InLast.SlotPosition = InJsonObject->GetIntegerField(TEXT("SlotPosition"));
					InLast.LegSize = InJsonObject->GetNumberField(TEXT("LegSize"));
					InLast.WaistSize = InJsonObject->GetNumberField(TEXT("WaistSize"));
					InLast.ArmSize = InJsonObject->GetNumberField(TEXT("ArmSize"));
				}
			}
		}
	}

	void StringToCharacterAppearances(const FString& OutString, FMMOARPGCharacterAppearance& InCAs)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(OutString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			InCAs.Name = ReadRoot->GetStringField(TEXT("Name"));
			InCAs.Date = ReadRoot->GetStringField(TEXT("Date"));
			InCAs.Lv = ReadRoot->GetIntegerField(TEXT("Lv"));
			InCAs.SlotPosition = ReadRoot->GetIntegerField(TEXT("SlotPosition"));
			InCAs.LegSize = ReadRoot->GetNumberField(TEXT("LegSize"));
			InCAs.WaistSize = ReadRoot->GetNumberField(TEXT("WaistSize"));
			InCAs.ArmSize = ReadRoot->GetNumberField(TEXT("ArmSize"));
		}
	}
}

void FMMOARPGUserData::Reset()
{
	ID = INDEX_NONE;

	Account.Empty();
	EMail.Empty();
	Name.Empty();
	HeadPortraitURL.Empty();
}
