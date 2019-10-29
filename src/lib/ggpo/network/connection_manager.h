#pragma once

class ConnectionManager {
public:
	virtual int SendTo(char* buffer, int len, int flags, int connection_id) = 0;
	//struct sockaddr* dst, int destlen store these in a udp class

	virtual int RecvFrom(char* buffer, int len, int flags, int connection_id) = 0;

	virtual int ResetManager() = 0;

};


class UDPConnectionManager : ConnectionManager {

public:
	UDPConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int recvfrom(char* buffer, int len, int flags, int& connection_id);

	//returns the connection id of the added connection;
	virtual int AddConnection(struct sockaddr* dst, int destlen);
};

class SteamConnectionManager : ConnectionManager {

public:
	SteamConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int recvfrom(char* buffer, int len, int flags, int connection_id);

	//returns the connection id of the added connection;
	virtual int AddConnection(/*HSteamNetConnection connection*/);
};
