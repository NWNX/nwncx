#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <WinNT.h>
#include "CGameSpyStub.h"

CGameSpyStub::CGameSpyStub(FILE * logFile)
{
	this->logFile = logFile;
	currentState = GS_STATE_IDLE;
}

CGameSpyStub::~CGameSpyStub()
{

}

CGameSpyStub::ClientState CGameSpyStub::getCurrentState()
{
	return currentState;
}

void CGameSpyStub::peerThink()
{
	fprintf(logFile, "- peerThink: %d\n", currentState);	fflush(logFile);
	switch(currentState)
	{
	case GS_STATE_IDLE:
		break;

	case GS_STATE_CONNECTING:
		if(pConnectCallback)
		{
			pConnectCallback(
					(PVOID) 0xFEFE0000,
					PEER_TRUE,
					getConnectionLib()
					);
		}
		break;

	case GS_STATE_LISTING_GROUPS:
		if(pListGroupRoomsCallback)
		{
			const int GroupIds[] =
			{
				274,
				363,
				364,
				275,
				276,
				365,
				366,
				277,
				279,
				278,
				367,
				368,
				370
			};

			const char * RoomNames[] =
			{
				"Action",
				"Story",
				"StoryLite",
				"RolePlay",
				"TeamNvsN",
				"Melee1vsN",
				"Arena1vsN",
				"Social",
				"Alternative",
				"PWAction",
				"PWStory",
				"Solo",
				"TechSupport"
			};

			for (size_t i = 0; i < RTL_NUMBER_OF(GroupIds); i++)
			{
				pListGroupRoomsCallback(
					(PVOID) 0xFEFB0000,
					PEER_TRUE,
					GroupIds[i],
					(PVOID) 0xFEFB0000,
					RoomNames[i],
					0,
					0,
					0,
					0,
					getConnectionLib()					
					);
			}

			pListGroupRoomsCallback(
				(PVOID) 0xFEFBE000,
				PEER_TRUE,
				0,
				(PVOID) 0xFEFBF000,
				"",
				0,
				0,
				0,
				0,
				getConnectionLib()
				);
		}
		currentState = GS_STATE_IDLE;
		break;

	case GS_STATE_JOINING_GROUP:
		if(pJoinGroupRoomCallback)
		{
			pJoinGroupRoomCallback(
				(PVOID) 0xFEFC0000,
				PEER_TRUE,
				0,
				0,
				getConnectionLib()
				);

		}
		break;

	case GS_STATE_LISTING_GAMES:
		if(pListingGamesCallback)
		{
			pListingGamesCallback(
				(PVOID) 0xFEFD0000,
				PEER_TRUE,
				(const char *) 0xFEFD1000,
				(PVOID) 0xFEFD2000,
				PEER_TRUE,
				3,
				0,
				getConnectionLib()
				);
		}
		currentState = GS_STATE_IDLE;
		break;
	}
	//currentState = GS_STATE_IDLE;
}

void CGameSpyStub::peerConnect(ConnectCallbackProc callback, CConnectionLib *connectionLib)
{
	pConnectCallback = callback;
	currentState = GS_STATE_CONNECTING;
}

void CGameSpyStub::peerListGroupRooms(ListGroupRoomsCallbackProc callback)
{
	pListGroupRoomsCallback = callback;
	currentState = GS_STATE_LISTING_GROUPS;
}

void CGameSpyStub::peerJoinGroupRoom(JoinGroupRoomCallbackProc callback)
{
	pJoinGroupRoomCallback = callback;
	currentState = GS_STATE_JOINING_GROUP;
}

void CGameSpyStub::peerStartListingGames(ListingGamesCallbackProc callback)
{
	pListingGamesCallback = callback;
	currentState = GS_STATE_LISTING_GAMES;
}

CConnectionLib *CGameSpyStub::getConnectionLib()
{
	return g_pAppManager->ClientExoApp->Internal->m_pConnectionLib;
}