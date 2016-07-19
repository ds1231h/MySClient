/*
My socket client
version 1.0
connect with My sokcet server 1.0
MuPei
2016.07.19
function: client for only one server
*/

/*
step:
1.initial socket dll
2.creat socket
3.connect server
4.send data to server
5.receive data from server
6.exit
*/

#include <stdafx.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main()
{
	WSADATA wsaD;
	SOCKET sClient;
	sockaddr_in servAddr;
	const int BUFSIZE = 1024;
	const int portId = 1996;
	char sendBuf[BUFSIZE];
	char resvBuf[BUFSIZE];
	int retVal;

	// initial socket dll
	if (WSAStartup(MAKEWORD(2, 2), &wsaD) != 0)
	{
		cout << "WSA startup failed!" << endl;
		return -1;
	}

	// creat socket
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sClient)
	{
		cout << "crear socket failed!" << endl;
		WSACleanup();
		return -1;
	}
	
	// server socket address
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portId);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connect server
	retVal = connect(sClient, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sClient);
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "connect succeed!" << endl;
	}

	while(TRUE)
	{
		// send data to server
		ZeroMemory(sendBuf, BUFSIZE);
		cout << "send data to server:";
		cin >> sendBuf;
		retVal = send(sClient, sendBuf, strlen(sendBuf), 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "send data failed!" << endl;
			closesocket(sClient);
			WSACleanup();
			return -1;
		}

		// receive data from server
		recv(sClient, resvBuf, BUFSIZE, 0);
		cout << "receive data succeed!" << endl;
		cout << "receive from server:" << resvBuf << endl;
	}

	// exit
	closesocket(sClient);
	WSACleanup();
	return 0;
}
