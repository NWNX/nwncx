#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif
#pragma comment(lib, "Ws2_32.lib")


#include <stdio.h>
#include <string.h>

#define NEW_MASTER "nwn1.mst.valhallalegends.com"

#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
#include "soap/WSHttpBinding_USCOREINWNMasterServerAPI.nsmap"

#include "NWNMSClient.h"

typedef WSHttpBinding_USCOREINWNMasterServerAPIProxy NWNMasterServerAPIProxy;
typedef _ns1__GetOnlineServerList GetOnlineServerList;
typedef _ns1__GetOnlineServerListResponse GetOnlineServerListResponse;
typedef ns4__ArrayOfNWGameServer ArrayOfNWGameServer;
typedef ns4__NWGameServer NWGameServer;
const char *endpoint = "http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX";

NWNMSClient::NWNMSClient(FILE *logFile)
{
	this->logFile = logFile;
}

void NWNMSClient::GetServersInRoom(int nRoom)
{
	int res, numServers;
	NWNMasterServerAPIProxy   *listserver = NULL;
	GetOnlineServerList *srv_list = NULL;
	GetOnlineServerListResponse *srv_response = NULL;
	ArrayOfNWGameServer *game_list = NULL;
	NWGameServer *game = NULL;


	
	listserver = new NWNMasterServerAPIProxy();
	listserver->soap_endpoint = endpoint;
	//listserver->

	srv_list = new GetOnlineServerList();
	srv_list->Product = (char *)malloc(sizeof(char) * 5);  // NWN1 plus a null character.
	strcpy_s(srv_list->Product, sizeof(char) * 5,  "NWN1");
	
	srv_response = new GetOnlineServerListResponse();

	res = listserver->GetOnlineServerList(srv_list, srv_response);
	/*if(SOAPErrorCheck(res)) {
		// You didn't think I would forget about those 5 bytes, did you?
		free(srv_list->Product);
		return;
	}*/

	
	game_list = srv_response->GetOnlineServerListResult;
	if(game_list == NULL) {
		MessageBoxA(NULL, "Server Listing Failed.  Better luck next time!", "Error", MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);		
		free(srv_list->Product);
		return;
	}
	
	numServers = game_list->__sizeNWGameServer;
	fprintf(logFile, "Got servers: %d\n", numServers);
	fflush(logFile);
}

const char * NWNMSClient::GetErrorMessage(int res)
{

}