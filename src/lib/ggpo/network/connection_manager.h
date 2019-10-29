#pragma once

#include <map>
#include <memory>

class ConnectionInfo {};

class ConnectionManager {
public:
	ConnectionManager();
	virtual int SendTo(char* buffer, int len, int flags, int connection_id) = 0;
	//struct sockaddr* dst, int destlen store these in a udp class

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id) = 0;

	//returns the connection id of the added connection;
	virtual int AddConnection(std::shared_ptr<ConnectionInfo> info) {
		connection_map.insert(std::pair<int, std::shared_ptr<ConnectionInfo>> (_id_to_issue, info));
		return _id_to_issue++;
	}

	virtual int ResetManager() {
		connection_map.clear();
		return 0;
	}

protected:
	int _id_to_issue;
	std::map <int, std::shared_ptr<ConnectionInfo>> connection_map;
};


class udp_info : ConnectionInfo   {
public:
	udp_info(char* ip_address, short port) {
		std::copy(ip_address, ip_address+32, this->ip_address);
		this->port = port;
	}
	char        ip_address[32];
	short       port;
};

class steam_info : ConnectionInfo {
	/*HSteamNetConnection connection*/
};

class UDPConnectionManager : ConnectionManager {

public:
	UDPConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id);

	int AddConnection(char* ip_address, short port);

protected:
	std::shared_ptr<ConnectionInfo> BuildConnectionInfo(char* ip_address, short port);

	sockaddr_in _peer_addr;


};

class SteamConnectionManager : ConnectionManager{

public:
	SteamConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id);
};
