/*
My socket client
version 1.2
connect with My sokcet server 1.2
MuPei
2016.07.21
function: client for one server
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
bool sendFinish = false;

// receive data
void recvData(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char recvBuf[BUFSIZE] = {0};
	
	int nCount = 0; // for receive file
	HANDLE hEvent = NULL;
	DWORD dReturn;

	while (TRUE)
	{
		if (sendFinish)
		{
			hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			dReturn = WaitForSingleObject(hEvent,INFINITE);
		} 
		else
		{
			hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			dReturn = WaitForSingleObject(hEvent,10000); // after 10s, disconnect
		}
	
		if (dReturn == WAIT_OBJECT_0)
		{
			memset(recvBuf, 0, BUFSIZE+5);
			nCount = recv(*sServer, recvBuf, BUFSIZE+4, 0);
			if (SOCKET_ERROR == nCount)
			{
				cout << "accetp data failed!" << endl;
				closesocket(*sServer);
				WSACleanup();
				return ;
				continue;
			}
			else
			{
				// receive data from server
				cout << "receive from server:" << recvBuf << endl;
				memset(recvBuf, 0, BUFSIZE);
			}
		}
		else if (dReturn == WAIT_FAILED)
		{
			cout << "error!" << endl;
		}
	} 
	

	// exit
	closesocket(*sServer);
	WSACleanup();
	return ;
}

void recvFile(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char recvBuf[BUFSIZE+5] = {0};
	char bufTop[6] = {0};
	char sendError[] = {"send error, please send again"};
	int nCount = 0; // for receive file
	int nData = 0;
	char* szBuff = NULL;

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

	WIN32_FIND_DATA FileInfo;
	recv(*sServer, (char*)&FileInfo, sizeof(WIN32_FIND_DATA), 0);
	while (nCount < FileInfo.nFileSizeLow)
	{
		szBuff = new char[1024];
		memset(szBuff, 0, 1024);
		nData = recv(*sServer, szBuff, 1024, 0);
		fwrite(szBuff, 1, nData, fp);
		nCount += nData;
	}
	delete szBuff;
	fclose(fp);
	cout << "receive file succeed!" << endl;
// 	while (TRUE)
// 	{
// 		memset(recvBuf, 0, BUFSIZE+5);
// 		nCount = recv(*sServer, recvBuf, BUFSIZE+4, 0);
// 		if (SOCKET_ERROR == nCount)
// 		{
// 			cout << "accetp data failed!" << endl;
// 			closesocket(*sServer);
// 			WSACleanup();
// 			return ;
// 			continue;
// 		}
// 
// 		// receive file
// 		for (int i = 0; i < 5; i++)
// 		{
// 			bufTop[i] = recvBuf[i];
// 		}
// 		// bufTop[6] = '\0';
// 		if (!strcmp(bufTop, "f1996"))
// 		{
// 			fwrite(recvBuf+5, 1, BUFSIZE-1, fp);
// 		}
// 		else if (!strcmp(bufTop, "1996e")) // transform end
// 		{
// 			cout << "receive file succeed!" << endl;
// 			fclose(fp);
// 			sendFinish = true;
// 			break;
// 		}
// 		else
// 		{
// 			// there are something wrong, send again
// 			send(*sServer, sendError, sizeof(sendError), 0);
// 			continue;
// 		}
//	}
	// receive finish

	// exit
// 	closesocket(*sServer);
// 	WSACleanup();
	return ;
}

// send data
void sendData(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char sendBuf[BUFSIZE] = {0};
	int retVal = 0;
	HANDLE hEvent = NULL;
	DWORD dReturn;

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	while (TRUE)
	{
		dReturn = WaitForSingleObject(hEvent,10000); // after 10s, disconnect

		if (dReturn == WAIT_OBJECT_0)
		{
			cout << "please input your name: ";
			cin >> sendBuf;
			cout << "key word: ";
			cin >> sendBuf+10;
			retVal = send(*sServer, sendBuf, sizeof(sendBuf), 0);

			while (SOCKET_ERROR == retVal)
			{
				cout << "upload failed!" << endl;
				cout << "please input again" << endl;
				cout << "please input your name: ";
				cin >> sendBuf;
				cout << "key word: ";
				cin >> sendBuf+10;
				retVal = send(*sServer, sendBuf, strlen(sendBuf), 0);
			}

			// send data to server
			cin >> sendBuf;
			retVal = send(*sServer, sendBuf, strlen(sendBuf), 0);
			if (SOCKET_ERROR == retVal)
			{
 				cout << "send data failed!" << endl;
// 	 			closesocket(*sServer);
// 	 			WSACleanup();
// 	 			return ;
				continue;
			}
		}
		else if (dReturn == WAIT_FAILED)
		{
			cout << "error!" << endl;
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
	uintptr_t threadId0 = _beginthread(recvFile, 0, &sServer); // receive file
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