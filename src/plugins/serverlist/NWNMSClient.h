#ifndef NWNMSClient_H
#define NWNMSClient_H

class NWNMSClient
{
public:
	NWNMSClient(FILE *logFile);

	void GetServersInRoom(int nRoom);
	const char * GetErrorMessage(int res);


private:
	FILE *logFile;

};

#endif