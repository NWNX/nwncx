#ifndef NWNMSClient_H
#define NWNMSClient_H
#pragma once


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

#include <nwnapi\CAppManager.h>

class NWNMSClient
{
public:
	NWNMSClient(FILE *logFile, CAppManager *app);
	~NWNMSClient();

	// Functions
	void AddServers();
	void GetServersInRoom(int nRoom);
	const char * GetErrorMessage(int res);


	// Data

	//DWORD threadId;



private:
	int RoomToSkywing(int room);

	int room;
	FILE *logFile;
	CAppManager *AppManager;

	NWNMasterServerAPIProxy   *listserver;
	NWGameServer *game;
	ArrayOfNWGameServer *servers;
	GameLookup *srv_list;
	GameLookupResponse *srv_response;


};

#endif