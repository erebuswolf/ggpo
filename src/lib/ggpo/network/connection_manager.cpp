
#include "types.h"
#include "connection_manager.h"

ConnectionManager::ConnectionManager() : _id_to_issue(0) {}

UDPConnectionManager::UDPConnectionManager() {}

int UDPConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {

//	ASSERT(entry.dest_addr.sin_addr.s_addr);

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

int UDPConnectionManager::RecvFrom(char* buffer, int len, int flags, int* connection_id) {
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

int UDPConnectionManager::AddConnection(char* ip_address, short port) {
	/*
	_peer_addr.sin_family = AF_INET;
	_peer_addr.sin_addr.s_addr = inet_addr(ip);
	_peer_addr.sin_port = htons(port);
	*/

	sockaddr_in dest_addr;
	return ConnectionManager::AddConnection(BuildConnectionInfo(ip_address, port));
}

std::shared_ptr<ConnectionInfo> UDPConnectionManager::BuildConnectionInfo(char* ip_address, short port) {

	return std::shared_ptr<ConnectionInfo>((ConnectionInfo*) new udp_info(ip_address, port));
}

//////////////////// STEAM Connection

SteamConnectionManager::SteamConnectionManager() {}

int SteamConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {

	return -1;
}

int SteamConnectionManager::RecvFrom(char* buffer, int len, int flags, int* connection_id) {
	return -1;
}
