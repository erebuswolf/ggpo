
#include "types.h"
#include "connection_manager.h"


UDPConnectionManager::UDPConnectionManager() {}


int UDPConnectionManager::AddConnection(struct sockaddr* dst, int destlen) {
	return -1;
}

int UDPConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {
	/*
	struct sockaddr_in* to = (struct sockaddr_in*)dst;

	int res = sendto(_socket, buffer, len, flags, dst, destlen);
	if (res == SOCKET_ERROR) {
		DWORD err = WSAGetLastError();
		DWORD e2 = WSAENOTSOCK;
		Log("unknown error in sendto (erro: %d  wsaerr: %d).\n", res, err);
		ASSERT(FALSE && "Unknown error in sendto");
	}
	Log("sent packet length %d to %s:%d (ret:%d).\n", len, inet_ntoa(to->sin_addr), ntohs(to->sin_port), res);
	*/
	return 0;
}

int UDPConnectionManager::recvfrom(char* buffer, int len, int flags, int& connection_id) {
	//set the connection id to the connection we recieved the data from.

	// resolve the address via the connection ID
	//recvfrom(_socket, (char*)recv_buf, MAX_UDP_PACKET_SIZE, 0, (struct sockaddr*) & recv_addr, &recv_addr_len);

	  // TODO: handle len == 0... indicates a disconnect.
	/*
	if (len == -1) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			Log("recvfrom WSAGetLastError returned %d (%x).\n", error, error);
		}
		break;
	} else if (len > 0) {
		Log("recvfrom returned (len:%d  from:%s:%d).\n", len, inet_ntoa(recv_addr.sin_addr), ntohs(recv_addr.sin_port));
		UdpMsg* msg = (UdpMsg*)recv_buf;
		_callbacks->OnMsg(recv_addr, msg, len);
	}*/
	return 0;
}


SteamConnectionManager::SteamConnectionManager() {}

int SteamConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {
	return -1;
}

int SteamConnectionManager::recvfrom(char* buffer, int len, int flags, int connection_id) {
	return -1;
}

//returns the connection id of the added connection;
int AddConnection(/*HSteamNetConnection connection*/) {
	return -1;
}