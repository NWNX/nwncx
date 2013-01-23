#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif
#pragma comment(lib, "Ws2_32.lib")


#include <stdio.h>
#include <string.h>

// [addicted2rpg] This is useful for redirecting the client to a dummmy master server, although no function is 
// performed as of present (that I know of), although the connect succeeds and has a similar effect as the 
// disablems plugin.  Currently unused.
#define NEW_MASTER "nwn1.mst.valhallalegends.com"


#include "NWNMSClient.h"
#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
#include "soap/WSHttpBinding_USCOREINWNMasterServerAPI.nsmap"


// Turn gSOAP generated names into more human friendly names.
typedef WSHttpBinding_USCOREINWNMasterServerAPIProxy NWNMasterServerAPIProxy;
typedef _ns1__GetOnlineServerList GetOnlineServerList;
typedef _ns1__GetOnlineServerListResponse GetOnlineServerListResponse;
typedef ns4__ArrayOfNWGameServer ArrayOfNWGameServer;
typedef ns4__NWGameServer NWGameServer;
typedef _ns1__LookupServerByGameType GameLookup;
typedef _ns1__LookupServerByGameTypeResponse GameLookupResponse;
const char *endpoint = "http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX";





NWNMSClient::NWNMSClient(FILE *logFile)
{
	this->logFile = logFile;
}

ArrayOfNWGameServer *NWNMSClient::GetServersInRoom(int nRoom)
{
	int res, numServers;
	NWNMasterServerAPIProxy   *listserver = NULL;
	ArrayOfNWGameServer *game_list = NULL;
	NWGameServer *game = NULL;

//	GetOnlineServerListResponse *srv_response = NULL;
//	GetOnlineServerList *srv_list = NULL;
	GameLookup *srv_list = NULL;
	GameLookupResponse *srv_response;

	listserver = new NWNMasterServerAPIProxy();
	listserver->soap_endpoint = endpoint;

	srv_list = new GameLookup(); // new GetOnlineServerList();


	srv_list->Product = (char *)malloc(sizeof(char) * 5);  // NWN1 plus a null character.
	strcpy_s(srv_list->Product, sizeof(char) * 5,  "NWN1");	
	srv_list->GameType = (unsigned int *)malloc(sizeof(int));
	*(srv_list->GameType) = RoomToSkywing(nRoom);
	
	srv_response = new GameLookupResponse(); //new GetOnlineServerListResponse();
	
	// res = listserver->GetOnlineServerList(srv_list, srv_response);
	res = listserver->LookupServerByGameType(srv_list, srv_response);
	if(res != SOAP_OK) {
		//MessageBoxA(NULL, GetErrorMessage(res), "Error", MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);
		fprintf(logFile, GetErrorMessage(res));
		free(srv_list->Product);
		free(srv_list->GameType);
		return NULL;
	}
	
	
	game_list = srv_response->LookupServerByGameTypeResult; // srv_response->GetOnlineServerListResult;
	if(game_list == NULL) {
		//MessageBoxA(NULL, "This should never happen; The Gamelist is NULL - (no results, maybe?)", "Error", MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);		
		fprintf(logFile, "This should never happen; The Gamelist is NULL - (no results, maybe?)");
		free(srv_list->Product);
		free(srv_list->GameType);
		return NULL;
	}

	
	numServers = game_list->__sizeNWGameServer;
//	fprintf(logFile, "Got servers: %d\n", numServers);
//	fflush(logFile);

	free(srv_list->Product);
	free(srv_list->GameType);

	return game_list;
}

const char * NWNMSClient::GetErrorMessage(int res)
{

	if(soap_xml_error_check(res)) {
		return "Server Listing Failed.  It was an XML issue.\n";
	}
	else if(soap_soap_error_check(res)) {
		"Server Listing Failed.  SOAP itself had some kind of problem.\n";
	}
	else if(soap_tcp_error_check(res)) {
		"Server Listing Failed.  There was a TCP problem.\n";
	}
	else if(soap_ssl_error_check(res)) { 
		"Server Listing Failed.  There was a SSL problem.\n";
	}
	else if(soap_zlib_error_check(res)) { 
		"Server Listing Failed.  There was a ZLIB error!\n";
	}
	else if(soap_http_error_check(res)) {
		if(res == 415) {
			"Server Listing Failed.  Got back HTTP/Unsupported Media.  SOAP XML is probably for old bindings.\n";
		} else {
			"Server Listing Failed.  It was an HTTP error!\n";
		}
	}
	else {
		return "Server Listing Failed.  We have no idea why.  SOAP hates you.\n";
		
	}

	return "Undiagnosed error.\n";

}


	// The room list
	// Action       nRoom=274   *(c->NWGameServer[i]->GameType) = ?????; guessing its 12
	// Roleplay     nRoom=275   *(c->NWGameServer[i]->GameType) = 3;
	// Team         nRoom=276   *(c->NWGameServer[i]->GameType) = 4;
	// Social       nRoom=277   *(c->NWGameServer[i]->GameType) = 7;
	// PW Action    nRoom=278   *(c->NWGameServer[i]->GameType) = 9;
	// Alternative  nRoom=279   *(c->NWGameServer[i]->GameType) = 8;
	// Story        nRoom=363   *(c->NWGameServer[i]->GameType) = 1;
	// Story Lite   nRoom=364   *(c->NWGameServer[i]->GameType) = 2; 
	// Melee        nRoom=365   *(c->NWGameServer[i]->GameType) = 5;
	// Arena        nRoom=366   *(c->NWGameServer[i]->GameType) = 6;
	// PW Story     nRoom=367   *(c->NWGameServer[i]->GameType) = 10;
	// Solo         nRoom=368   *(c->NWGameServer[i]->GameType) = 11;
	// Tech Support nRoom=370   *(c->NWGameServer[i]->GameType) = ?????;  guessing its 13
int NWNMSClient::RoomToSkywing(int room) {

	switch(room) {
	case 274:
		return 12; break;
	case 275:
		return 3; break;
	case 276:
		return 4; break;
	case 277:
		return 7; break;
	case 278:
		return 9; break;
	case 279:
		return 8; break;
	case 363:
		return 1; break;
	case 364:
		return 2; break;
	case 365:
		return 5; break;
	case 366:
		return 6; break;
	case 367:
		return 10; break;
	case 368:
		return 11; break;
	case 370:
		return 13; break;
	default:
		return 3; break;  // just put it in Roleplay :P
	}

}
