#include "MyqlConfig.h"
#include "Misc/FileHelper.h"

FSimpleMysqlConfig *FSimpleMysqlConfig::Global = nullptr;

FSimpleMysqlConfig * FSimpleMysqlConfig::Get()
{
	if (!Global)
	{
		Global = new FSimpleMysqlConfig();
	}

	return Global;
}

void FSimpleMysqlConfig::Destroy()
{
	if (Global)
	{
		delete Global;
		Global = nullptr;
	}
}

const FMysqlConfig& FSimpleMysqlConfig::GetInfo() const
{
	return ConfigInfo;
}

void FSimpleMysqlConfig::Init(const FString& InPath)
{
	TArray<FString> Content;
	FFileHelper::LoadFileToStringArray(Content ,*InPath);
	if (Content.Num())
	{
		auto Lamabda = [&](TMap<FString,FString> &OutContent)
		{
			for (auto &Tmp : Content)
			{
				if (Tmp.Contains("[") && Tmp.Contains("]"))//解析头部
				{
					Tmp.RemoveFromEnd("]");
					Tmp.RemoveFromStart("[");
				
					OutContent.Add("ConfigHead", Tmp);
				}
				else //解析身体
				{
					FString R, L;
					Tmp.Split(TEXT("="), &L, &R);

					OutContent.Add(L, R);
				}
			}
		};

		TMap<FString, FString> InConfigInfo;
		Lamabda(InConfigInfo);

		ConfigInfo.User = InConfigInfo["User"];
		ConfigInfo.Host = InConfigInfo["Host"];
		ConfigInfo.Pawd = InConfigInfo["Pawd"];
		ConfigInfo.DB = InConfigInfo["DB"];
		ConfigInfo.Port = FCString::Atoi(*(InConfigInfo["Port"]));
	}
	else
	{
		Content.Add(TEXT("[SimpleMysqlConfig]"));
		Content.Add(FString::Printf(TEXT("User=%s"), *ConfigInfo.User));
		Content.Add(FString::Printf(TEXT("Host=%s"), *ConfigInfo.Host));
		Content.Add(FString::Printf(TEXT("Pawd=%s"), *ConfigInfo.Pawd));
		Content.Add(FString::Printf(TEXT("DB=%s"), *ConfigInfo.DB));
		Content.Add(FString::Printf(TEXT("Port=%i"), ConfigInfo.Port));

		FFileHelper::SaveStringArrayToFile(Content, *InPath);
	}
}