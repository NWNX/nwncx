#ifndef CGameSpyStub_H
#define CGameSpyStub_H
#pragma once

#include "nwnapi\CAppManager.h"
#include "nwnapi\CConnectionLib.h"
#include "nwnapi\custom\nwn_globals.h"

class CGameSpyStub
{
public:
	enum ClientState {
		GS_STATE_IDLE,
		GS_STATE_CONNECTING,
		GS_STATE_LISTING_GROUPS,
		GS_STATE_JOINING_GROUP,
		GS_STATE_LISTING_GAMES
	};
	enum PEERBool
	{
		PEER_FALSE,
		PEER_TRUE
	};

	typedef void (__cdecl * ConnectCallbackProc) (
		void *,
		enum PEERBool,
		void *  //ConnectionLib
		);

	typedef void (__cdecl * ListGroupRoomsCallbackProc) (
		void *,
		enum PEERBool,
		int,
		void *,       //_SBServer server
		char const *, //RoomName
		int,
		int,
		int,
		int,
		void *  //ConnectionLib
		);

	typedef void (__cdecl * JoinGroupRoomCallbackProc) (
		void *,
		enum PEERBool,
		int,
		int,
		void *  //ConnectionLib
		);

	typedef void (__cdecl * ListingGamesCallbackProc) (
		void *,
		enum PEERBool,
		char const *,
		void *,       //_SBServer server
		enum PEERBool,
		int, //Command
		int,
		void *  //ConnectionLib
		);


	CGameSpyStub(FILE * logFile);
	~CGameSpyStub();
	ClientState getCurrentState();

	void peerConnect(ConnectCallbackProc callback, CConnectionLib *connectionLib);
	void peerListGroupRooms(ListGroupRoomsCallbackProc callback);
	void peerJoinGroupRoom(JoinGroupRoomCallbackProc callback);
	void peerStartListingGames(ListingGamesCallbackProc callback);
	void peerThink();

private:
	FILE * logFile;
	ClientState currentState;

	ConnectCallbackProc pConnectCallback;
	ListGroupRoomsCallbackProc pListGroupRoomsCallback;
	JoinGroupRoomCallbackProc pJoinGroupRoomCallback;
	ListingGamesCallbackProc pListingGamesCallback;

	CConnectionLib *getConnectionLib();
};

#endif