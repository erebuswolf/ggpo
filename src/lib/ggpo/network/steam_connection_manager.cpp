
#include "types.h"
#include "steam_connection_manager.h"

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