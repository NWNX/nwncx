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


const char *endpoint = "http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX";





NWNMSClient::NWNMSClient(FILE *logFile, CAppManager *app)
{
	this->logFile = logFile;
	AppManager = app;

	listserver = NULL;
	game = NULL;
	servers = NULL;
	srv_list = NULL;
	srv_response = NULL;

}

NWNMSClient::~NWNMSClient() {
	if(listserver != NULL) delete listserver;

	if(srv_list!= NULL) {
		if(srv_list->Product != NULL) {
			free(srv_list->Product);
		}
		if(srv_list->GameType != NULL) {
			free(srv_list->GameType);
		}
		delete srv_list;
	}

	if(srv_response != NULL) delete srv_response;

	// servers is actually a pointer to srv_response, so nothing to free.



}

// should be called from child thread
void NWNMSClient::GetServersInRoom(int nRoom)
{
	int res, numServers;

	this->room = nRoom;

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
		return;
	}
	
	
	servers = srv_response->LookupServerByGameTypeResult; // srv_response->GetOnlineServerListResult;
	if(servers == NULL) {
		//MessageBoxA(NULL, "This should never happen; The Gamelist is NULL - (no results, maybe?)", "Error", MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR | MB_OK);		
		fprintf(logFile, "This should never happen; The Gamelist is NULL - (no results, maybe?)");
		free(srv_list->Product);
		free(srv_list->GameType);
		return;
	}

	
	numServers = servers->__sizeNWGameServer;
//	fprintf(logFile, "Got servers: %d\n", numServers);
//	fflush(logFile);

	free(srv_list->Product);
	free(srv_list->GameType);

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
	// Action       nRoom=274   *(c->NWGameServer[i]->GameType) = 13;
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
	// Tech Support nRoom=370   *(c->NWGameServer[i]->GameType) = 12;
int NWNMSClient::RoomToSkywing(int room) {

	switch(room) {
	case 274:
		return 13; break;
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
		return 12; break;
	default:
		return 3; break;  // just put it in Roleplay :P
	}

}



// Now called by the main thread....
void NWNMSClient::AddServers() {
	int i, portnumber;
	char *pvpfull = "FULL";
	char *pvpparty = "PARTY";
	char *pvpnone = "NONE";
	char *str_ptr;
	char buildstring[10];
	char *IP_ptr, *port_ptr;

	// The original function has a "Clear" routine that seems to be run after it.  I want this to run 
	// after that routine, so that is what the wait is about.  It works too.... after a fashion.
	// Sleep(500);


	this->AppManager->ClientExoApp->Internal->m_pConnectionLib->ClearServers();

	for(i=0; i < servers->__sizeNWGameServer;i++) {
		if(*(servers->NWGameServer[i]->Online) != 1) continue;

		IP_ptr = strtok(servers->NWGameServer[i]->ServerAddress, ":");
		port_ptr = IP_ptr + strlen(IP_ptr) + 1;   // I never really trusted strtok(NULL) anyway :)

		if(*(servers->NWGameServer[i]->PVPLevel) == 0) {
			str_ptr = pvpnone;
		}
		else if(*(servers->NWGameServer[i]->PVPLevel) == 1) {
			str_ptr = pvpparty;
		}
		else {
			str_ptr = pvpfull;
		}
		itoa(*(servers->NWGameServer[i]->BuildNumber), buildstring, 10);
		portnumber = atoi(port_ptr);
//		fprintf(logFile, "Online: %d Server Address: %s  Port Number: %s Room: %d\n", *(servers->NWGameServer[i]->Online), IP_ptr, port_ptr, *(servers->NWGameServer[i]->GameType));		

		this->AppManager->ClientExoApp->Internal->m_pConnectionLib->AddServer(
			(void *)i, 
			servers->NWGameServer[i]->ServerName, 
			servers->NWGameServer[i]->ModuleName,
			*(servers->NWGameServer[i]->ActivePlayerCount),
			*(servers->NWGameServer[i]->MaximumPlayerCount),
			*(servers->NWGameServer[i]->MinimumLevel),
			*(servers->NWGameServer[i]->MaximumLevel),
			str_ptr,
			75, // making up ping initially...
			*(servers->NWGameServer[i]->PrivateServer), 
			IP_ptr,
			portnumber, 
			*(servers->NWGameServer[i]->OnePartyOnly),
			*(servers->NWGameServer[i]->PlayerPause),
			buildstring, 
			servers->NWGameServer[i]->ServerDescription, 
			this->room, // I think this is groupID
			*(servers->NWGameServer[i]->ELCEnforced),
			*(servers->NWGameServer[i]->ILREnforced),
			*(servers->NWGameServer[i]->LocalVault),
			*(servers->NWGameServer[i]->ExpansionsMask),
			false);

	}


	this->AppManager->ClientExoApp->Internal->m_pConnectionLib->UpdateConnectionPhase(9, "");

	fprintf(logFile, "%d Servers added\n", i);
	fflush(logFile);

}
