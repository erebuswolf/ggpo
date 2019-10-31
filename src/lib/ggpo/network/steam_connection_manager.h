
#ifndef _STEAM_CONNECTION_MANAGER_H
#define _STEAM_CONNECTION_MANAGER_H
#include "connection_manager.h"

class steam_info : public ConnectionInfo {
	/*HSteamNetConnection connection*/
};

class SteamConnectionManager : public ConnectionManager {

public:
	SteamConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id);

	virtual std::string ToString(int connection_id);
};

#endif   
