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
	SOCKET sServer;
	sockaddr_in servAddr;
	const int BUFSIZE = 1019;
	const int portId = 1996;
	char sendBuf[BUFSIZE] = {0};
	char recvBuf[BUFSIZE+5] = {0};
	char bufTop[5] = {0};
	int retVal = 0;
	int nCount = 0; // for receive file

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

	// receive file from server
	// check if the file could create
	char* filename = "E:\\sqlmap.rar";
	// char* filename = "D:\\test.txt";
	FILE* fp = fopen(filename, "wb");
	if (NULL == fp)
	{
		cout << "create file failed!" << endl;
		return -1;
	}

	// receive data from server
	while (TRUE)
	{
		memset(recvBuf, 0, BUFSIZE+5);
		nCount = recv(sServer, recvBuf, BUFSIZE+5, 0);
		// receive file
		//if (strcmp(recvBuf, "f1996") && strcmp(recvBuf+strlen(recvBuf)-1, "e1996")) // receive file
		for (int i = 0; i < 5; i++)
		{
			bufTop[i] = recvBuf[i];
		}
		bufTop[5] = '\0';
		if (!strcmp(bufTop, "f1996"))
		{
			fwrite(recvBuf+5, nCount-5, 1, fp);
		}
		//else if ((!strcmp(recvBuf, "f1996")) && strcmp(recvBuf+strlen(recvBuf)-1, "e1996"))
		else if (!strcmp(bufTop, "1996e")) // transform end
		{
			cout << "receive file succeed!" << endl;
			fclose(fp);
			continue;
		}
		else if ((strcmp(bufTop, "f1996")) || (strcmp(bufTop, "1996e")) && (strcmp(bufTop, "00000")))
		{
			// receive data from server
			// recv(sServer, recvBuf, BUFSIZE, 0);
			cout << "receive data succeed!" << endl;
			cout << "receive from server:" << recvBuf << endl;

			// send data to server
			cout << "send data to server:";
			cin >> sendBuf;
			retVal = send(sServer, sendBuf, strlen(sendBuf), 0);
			if (SOCKET_ERROR == retVal)
			{
				cout << "send data failed!" << endl;
				closesocket(sServer);
				WSACleanup();
				return -1;
			}
		}
		else
		{
			continue;
		}
	}

	// exit
	closesocket(sServer);
	WSACleanup();
	return 0;
}
