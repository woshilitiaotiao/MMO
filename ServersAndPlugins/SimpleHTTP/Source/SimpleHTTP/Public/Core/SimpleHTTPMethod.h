// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace SimpleHTTP
{
	//如果里面包含中文字符，会进行特殊处理，防止字符因为特殊导致HTTP错误
	FString SimpleURLEncode(const TCHAR* InUnencodedString);
}