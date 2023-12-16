#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "Core/SimpleMysqlLinkType.h"
#include "SimpleHTTPManage.h"
#include "MMOARPGType.h"
#include "MMOARPGServerObject.generated.h"

class USimpleMysqlObject;

UCLASS()
class UMMOARPGServerObject : public USimpleController
{
	GENERATED_BODY()

public:
	bool Post(const FString &InSQL);
	bool Get(const FString& InSQL, TArray<FSimpleMysqlResult>& Results);
public:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();
	virtual void RecvProtocol(uint32 InProtocol);

private:
	ECheckNameType CheckName(const FString& InName);
	void GetSerialString(TCHAR* InPrefix, const TArray<FString>& InStrings, FString& OutString);
private:
	bool GetUserInfo(int32 InUserID, FString& OutJsonString);
	bool GetSlotCAInfo(int32 InUserID, int32 InSlotCAID, FString& OutJsonString);
	bool GetCharacterCAInfoByUserMate(int32 InUserID, TArray<FString>& OutIDs);
protected:
	UFUNCTION()
	void CheckPasswordResult(const FSimpleHttpRequest& InRequest, const FSimpleHttpResponse& InResponse, bool bLinkSuccessful);

	UFUNCTION()
	void CheckRegisterResult(const FSimpleHttpRequest& InRequest, const FSimpleHttpResponse& InResponse, bool bLinkSuccessful);
protected:
	USimpleMysqlObject* MysqlObjectRead;
	USimpleMysqlObject* MysqlObjectWrite;
};