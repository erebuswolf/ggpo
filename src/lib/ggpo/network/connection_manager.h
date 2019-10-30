#pragma once

#include <map>
#include <memory>
#include <string>

class ConnectionInfo {
public:
	ConnectionInfo() {}
	void Log(const char* fmt, ...);
	virtual ~ConnectionInfo() {
		Log("destruction\n");
	}
	virtual int GetInt() { return 5; }
	virtual std::string ToString() = 0;
};

class ConnectionManager {
public:
	ConnectionManager() : _id_to_issue(0) {}
	virtual int SendTo(char* buffer, int len, int flags, int connection_id) = 0;
	//struct sockaddr* dst, int destlen store these in a udp class

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id) = 0;

	virtual std::string ToString(int connection_id) = 0;

	//returns the connection id of the added connection;
	virtual int AddConnection(std::shared_ptr<ConnectionInfo> info) {
		Log("making pair and inserting in map\n");
		connection_map.insert(std::make_pair (_id_to_issue, info));
		return _id_to_issue++;
	}

	virtual int ResetManager() {
		connection_map.clear();
		return 0;
	}

	void Log(const char* fmt, ...);

protected:
	int _id_to_issue;
	std::map <int, std::shared_ptr<ConnectionInfo>> connection_map;
};

class udp_info : public  ConnectionInfo   {
public:
	udp_info(char* ip_address, short port) {
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip_address);
		addr.sin_port = htons(port);
	}

	sockaddr_in addr;

	~udp_info() {
		Log("upd destruction\n");
	}

	virtual std::string ToString() {
		return std::string("UDP");
	}
};

class steam_info : ConnectionInfo {
	/*HSteamNetConnection connection*/
};

class UDPConnectionManager : public ConnectionManager {

public:
	UDPConnectionManager();
	virtual ~UDPConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id);

	virtual std::string ToString(int connection_id);

	int AddConnection(char* ip_address, short port);

	void Init(int port);

	int FindIDFromIP(sockaddr_in* sockaddr);


protected:
	std::shared_ptr<ConnectionInfo> BuildConnectionInfo(char* ip_address, short port);

	sockaddr_in _peer_addr;

	SOCKET _socket;

};

class SteamConnectionManager : ConnectionManager{

public:
	SteamConnectionManager();

	virtual int SendTo(char* buffer, int len, int flags, int connection_id);

	virtual int RecvFrom(char* buffer, int len, int flags, int* connection_id);

	virtual std::string ToString(int connection_id);
};
