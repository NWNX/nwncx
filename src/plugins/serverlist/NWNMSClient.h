#ifndef NWNMSClient_H
#define NWNMSClient_H
#pragma once


#ifndef soapWSHttpBinding_USCOREINWNMasterServerAPIProxy_H
#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
typedef ns4__ArrayOfNWGameServer ArrayOfNWGameServer;
#endif




class NWNMSClient
{
public:
	NWNMSClient(FILE *logFile);

	ArrayOfNWGameServer *GetServersInRoom(int nRoom);
	const char * GetErrorMessage(int res);
	int RoomToSkywing(int room);

	int room;
	DWORD threadId;


private:
	FILE *logFile;

};

#endif