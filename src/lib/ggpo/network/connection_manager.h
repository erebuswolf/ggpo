#pragma once

class ConnectionInfo {
	virtual void SendTo(char* buffer, int len, int flags, int connection_id) = 0;
	//struct sockaddr* dst, int destlen store these in a udp class
};

class ConnectionManager
{
	virtual void SendTo(char* buffer, int len, int flags, int connection_id) = 0;
	//struct sockaddr* dst, int destlen store these in a udp class

	virtual void AddConnection(int connection_id, ConnectionInfo *) = 0;
};


class UDPConnection : ConnectionInfo {
	UDPConnection();
	UDPConnection(struct sockaddr* dst, int destlen);

	virtual void SendTo(char* buffer, int len, int flags, int connection_id);

	struct sockaddr* dst;
	int destlen;
};