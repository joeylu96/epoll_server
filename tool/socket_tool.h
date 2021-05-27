#ifndef _SOCKET_TOOL_
#define _SOCKET_TOOL_

#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

class CSocketTool
{
	public:
		static int getTcpSocket(const char* ip, int port);
};

class CUserSocket
{
	public:
		int m_user_socket;
		long int m_update_time;
		char* buffer;
};

class CPackTool
{
	public:
		static int UnPack(string& req);
		static int Pack(string& rsp, char* str);
};

#endif