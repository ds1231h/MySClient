/*
My socket client
version 1.1
connect with My sokcet server 1.1
MuPei
2016.07.20
function: client for only one server
			receive from server
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

const int BUFSIZE = 1019;

// receive data
void recvData(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char recvBuf[BUFSIZE+5] = {0};
	char bufTop[5] = {0};
	int nCount = 0; // for receive file

	// receive file from server
	// check if the file could create
	char* filename = "E:\\sqlmap.rar";
	// char* filename = "D:\\test.txt";
	FILE* fp = fopen(filename, "wb");
	if (NULL == fp)
	{
		cout << "create file failed!" << endl;
		cout << "can't transform file!" << endl;
/*		return ;*/
	}

	while (TRUE)
	{
		memset(recvBuf, 0, BUFSIZE+5);
		nCount = recv(*sServer, recvBuf, BUFSIZE+5, 0);
		if (SOCKET_ERROR == nCount)
		{
			cout << "accetp data failed!" << endl;
// 			closesocket(*sServer);
// 			WSACleanup();
// 			return ;
			continue;
		}
		// receive file
		for (int i = 0; i < 5; i++)
		{
			bufTop[i] = recvBuf[i];
		}
		bufTop[5] = '\0';
		if (!strcmp(bufTop, "f1996"))
		{
			fwrite(recvBuf+5, nCount-5, 1, fp);
		}
		else if (!strcmp(bufTop, "1996e")) // transform end
		{
			cout << "receive file succeed!" << endl;
			fclose(fp);
		}
		else if ((strcmp(bufTop, "f1996")) || (strcmp(bufTop, "1996e")) && (strcmp(bufTop, "00000")))
		{
			// receive data from server
			cout << "receive from server:" << recvBuf << endl;
		}
	}
	// exit
	closesocket(*sServer);
	WSACleanup();
	return ;
}

// send data
void sendData(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char sendBuf[BUFSIZE] = {0};
	int retVal = 0;

	while (TRUE)
	{
		// send data to server
		cin >> sendBuf;
		retVal = send(*sServer, sendBuf, strlen(sendBuf), 0);
		if (SOCKET_ERROR == retVal)
		{
 			cout << "send data failed!" << endl;
// 			closesocket(*sServer);
// 			WSACleanup();
// 			return ;
			continue;
		}
	}
	// exit
	closesocket(*sServer);
	WSACleanup();
	return ;
}

int main()
{
	WSADATA wsaD;
	SOCKET sServer;
	sockaddr_in servAddr;
	int retVal = 0;
	const int portId = 1996;

	// initial socket dll
	if (WSAStartup(MAKEWORD(2, 2), &wsaD) != 0)
	{
		cout << "WSA startup failed!" << endl;
		return -1;
	}

	// creat socket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
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
	retVal = connect(sServer, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "connect succeed!" << endl;
	}

	// begin thread
	uintptr_t threadId1 = _beginthread(recvData, 0, &sServer); // receive
	uintptr_t threadId2 = _beginthread(sendData, 0, &sServer); // send
	while(TRUE)
	{

	}
	// exit
	closesocket(sServer);
	WSACleanup();
	return 0;
}
