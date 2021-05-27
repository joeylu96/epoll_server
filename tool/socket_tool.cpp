#include "socket_tool.h"

int CSocketTool::getTcpSocket(const char* ip, int port)
{
	int ret_sock=socket(AF_INET, SOCK_STREAM, 0);
	if(ret_sock==-1)
	{
		cout<<"CSocketTool::getTcpSocket get socket error"<<endl;
		return -1;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(ip);
	addr.sin_port=htons(port);
	if(bind(ret_sock, (struct sockaddr*)&addr, sizeof(addr))!=0)
	{
		cout<<"CSocketTool::getTcpSocket bind error"<<endl;
		return -2;
	}
	
	return ret_sock;
}

int CPackTool::UnPack(string& req)
{
	return 0;
}

int CPackTool::Pack(string& rsp, char* str)
{
	int len=rsp.length();
	memcpy(&str[4], &len, 4);
	
	string tmp="head";
	memcpy(str, tmp.c_str(), 4);
	
	return 8+len;
}