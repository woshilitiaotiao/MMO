#include "MMOARPGGameInstance.h"

#if PLATFORM_WINDOWS
#define SEND_DATA(InProtocols,...) \
if (UMMOARPGGameInstance* InGameInstance_A = GetGameInstance<UMMOARPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGameInstance_A->GetClient(), InProtocols,__VA_ARGS__); \
}
#else
#define SEND_DATA(InProtocols,args...) \
if (UMMOARPGGameInstance* InGameInstance_A = GetGameInstance<UMMOARPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGameInstance_A->GetClient(), InProtocols,##args); \
}
#endif