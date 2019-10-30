
#include "types.h"
#include "connection_manager.h"

void ConnectionInfo::Log(const char* fmt, ...)
{
	char buf[1024];
	size_t offset;
	va_list args;

	strcpy(buf, "ConnectionInfo | ");
	offset = strlen(buf);
	va_start(args, fmt);
	vsnprintf(buf + offset, ARRAYSIZE(buf) - offset - 1, fmt, args);
	buf[ARRAYSIZE(buf) - 1] = '\0';
	::Log(buf);
	va_end(args);
}

SOCKET
CreateSocket(int bind_port, int retries)
{
	SOCKET s;
	sockaddr_in sin;
	int port;
	int optval = 1;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);
	setsockopt(s, SOL_SOCKET, SO_DONTLINGER, (const char*)&optval, sizeof optval);

	// non-blocking...
	u_long iMode = 1;
	ioctlsocket(s, FIONBIO, &iMode);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	for (port = bind_port; port <= bind_port + retries; port++) {
		sin.sin_port = htons(port);
		if (bind(s, (sockaddr*)&sin, sizeof sin) != SOCKET_ERROR) {
			Log("Udp bound to port: %d.\n", port);
			return s;
		}
	}
	closesocket(s);
	return INVALID_SOCKET;
}

void ConnectionManager::Log(const char* fmt, ...)
{
	char buf[1024];
	size_t offset;
	va_list args;

	strcpy(buf, "connection_manager | ");
	offset = strlen(buf);
	va_start(args, fmt);
	vsnprintf(buf + offset, ARRAYSIZE(buf) - offset - 1, fmt, args);
	buf[ARRAYSIZE(buf) - 1] = '\0';
	::Log(buf);
	va_end(args);
}

UDPConnectionManager::UDPConnectionManager() : _socket(INVALID_SOCKET), _peer_addr() {}

int UDPConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {
	ASSERT(connection_map.count(connection_id));
	if (connection_map.count(connection_id) == 0) {
		Log("Connection not in map Connection ID: %d).\n", connection_id);
	}

	Log("trying to send %d).\n", connection_map.find(connection_id)->first);
	std::shared_ptr<ConnectionInfo> dest_addr = connection_map.find(connection_id)->second;
	Log("trying to send %d).\n", dest_addr->GetInt());

	if ((std::dynamic_pointer_cast <udp_info>(dest_addr)) == NULL) {
		Log("cast failed).\n");
		ASSERT(FALSE && "Cast failed");
	}

	int res = sendto(_socket, buffer, len, flags,
		(struct sockaddr*) & ((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr),
		sizeof(std::dynamic_pointer_cast <udp_info>(dest_addr)->addr));

	Log("trying to send %s).\n", ToString(connection_id).c_str());
	if (res == SOCKET_ERROR) {
		DWORD err = WSAGetLastError();
		DWORD e2 = WSAENOTSOCK;
		Log("unknown error in sendto (erro: %d  wsaerr: %d, Connection ID: %d).\n", res, err, connection_id);
		ASSERT(FALSE && "Unknown error in sendto");
	}

	Log("sent packet length %d to %s:%d (ret:%d).\n", len,
		inet_ntoa((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_addr),
		ntohs((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_port), res);
	
	return 0;
}

int UDPConnectionManager::RecvFrom(char* buffer, int len, int flags, int* connection_id) {
	//set the connection id to the connection we recieved the data from.

	// resolve the address via the connection ID
	
	sockaddr_in    recv_addr;
	int            recv_addr_len;
	recv_addr_len = sizeof(recv_addr);

	// >0 indicates data length.
	// 0 indicates a disconnect.
	// -1 indicates no data or some other error.
	int inlen = recvfrom(_socket, (char*)buffer, len, flags, (struct sockaddr*) & recv_addr, &recv_addr_len);

	*connection_id = FindIDFromIP(&recv_addr);

	// Platform specific error message handling should be done in the connection manager.
	if (inlen == -1) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			Log("recvfrom WSAGetLastError returned %d (%x).\n", error, error);
		}
	}

	return inlen;
}

int UDPConnectionManager::FindIDFromIP(sockaddr_in* addr) {

	for (std::map<int, std::shared_ptr<ConnectionInfo>>::iterator it = connection_map.begin();
		it != connection_map.end();
		++it) {
		std::shared_ptr<ConnectionInfo> dest_addr = it->second;
		if(
			((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_addr.S_un.S_addr == addr->sin_addr.S_un.S_addr)
			&&
			((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_port == addr->sin_port)) {
			return it->first;
		}
	}
	return -1;
}

void UDPConnectionManager::Init(int port) {
	_socket = CreateSocket(port, 0);
}

int UDPConnectionManager::AddConnection(char* ip_address, short port) {
	Log("adding connection start\n");
	auto connection = BuildConnectionInfo(ip_address, port);
	Log("building finished\n");
	int id = ConnectionManager::AddConnection(connection);
	Log("adding finished\n");
	return id;
}

std::string UDPConnectionManager::ToString(int connection_id) {
	char buffer[100];

	ASSERT(connection_map.count(connection_id));
	std::shared_ptr<ConnectionInfo> dest_addr = connection_map.find(connection_id)->second;

	sprintf(buffer, "Connection: IP: %s, Port: %d",
		inet_ntoa(((std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_addr)),
		(std::dynamic_pointer_cast <udp_info>(dest_addr))->addr.sin_port);
	
	return std::string(buffer);
}

UDPConnectionManager::~UDPConnectionManager() {
	if (_socket != INVALID_SOCKET) {
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
}

std::shared_ptr<ConnectionInfo> UDPConnectionManager::BuildConnectionInfo(char* ip_address, short port) {
	Log("building connection ptr\n");
	std::shared_ptr<udp_info> ptr= std::make_shared<udp_info>(ip_address, port);
	if (ptr == NULL) {
		Log("initial pointer sucks \n");
	}
	Log("built a thing %s \n", ptr->ToString().c_str());
	auto ptr2 = std::static_pointer_cast<ConnectionInfo>(ptr);
	if (ptr2 == NULL) {
		Log("pointer cast to parent failed \n");
	}
	return ptr2;
}

//////////////////// STEAM Connection

SteamConnectionManager::SteamConnectionManager() {}

int SteamConnectionManager::SendTo(char* buffer, int len, int flags, int connection_id) {

	return -1;
}

int SteamConnectionManager::RecvFrom(char* buffer, int len, int flags, int* connection_id) {
	return -1;
}


std::string SteamConnectionManager::ToString(int connection_id) {
	return std::string();
}