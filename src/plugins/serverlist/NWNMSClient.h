#ifndef NWNMSClient_H
#define NWNMSClient_H
#pragma once

#include <queue>

// [addicted2rpg] This is useful for redirecting the client to a dummmy master server, although no function is 
// performed as of present (that I know of), although the connect succeeds and has a similar effect as the 
// disablems plugin.  Currently unused.
#define NEW_MASTER "nwn1.mst.valhallalegends.com"
#define API_ENDPOINT "http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX"

#ifndef soapWSHttpBinding_USCOREINWNMasterServerAPIProxy_H
#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
// Turn gSOAP generated names into more human friendly names.
typedef WSHttpBinding_USCOREINWNMasterServerAPIProxy NWNMasterServerAPIProxy;
typedef _ns1__GetOnlineServerList GetOnlineServerList;
typedef _ns1__GetOnlineServerListResponse GetOnlineServerListResponse;
typedef ns4__ArrayOfNWGameServer ArrayOfNWGameServer;
typedef ns4__NWGameServer NWGameServer;
typedef _ns1__LookupServerByGameType GameLookup;
typedef _ns1__LookupServerByGameTypeResponse GameLookupResponse;

#endif

class NWNMSClient;
typedef struct TRequestThreadParams
{
	NWNMSClient *client;
	int roomId;
} RequestThreadParams;

typedef struct TServerListResult
{
	int roomId;
	NWNMasterServerAPIProxy *api;
	ArrayOfNWGameServer *servers;
} ServerListResult;

typedef void (*ServerListCallback_t)(ServerListResult);

class NWNMSClient
{
public:
	NWNMSClient(FILE *logFile, ServerListCallback_t serverListCallback);
	~NWNMSClient();

	// Functions
	void AddServers();
	void GetServersInRoom(int nRoom);
	const char * GetErrorMessage(int res);

	void                  RequestServerList(int roomId);
	void                  Update();
	void                  PushResult(ServerListResult result);
	ServerListResult      PopResult();
	bool                  HasResults();

	static DWORD WINAPI   RequestThread(void *param);

private:
	int RoomToSkywing(int room);

	FILE *                             logFile;
	int                                currentRoom;
	CRITICAL_SECTION                   cs;
	std::queue<ServerListResult>       resultQueue;

	ServerListCallback_t serverListCallback;
};

#endif