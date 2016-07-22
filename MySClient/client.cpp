/*
My socket client
version 1.3
connect with My sokcet server 1.3
MuPei
2016.07.22
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
const int RECVSIZE = 1024;

// receive data
void recvData(void* soc)
{
	SOCKET* sServer = (SOCKET*)soc;
	char recvBuf[RECVSIZE] = {0};
	char bufTop[5] = {0};
	int nCount = 0; // for receive file
	bool fileStart = true;
// 	union
// 	{
// 		int fsize;
// 		char fileSize[4];
// 	};
	char fileSize[4] = {0};
	char logData[50] = {"file receive "};
	unsigned int fsize;
	int restSize = 0;
	int transTime = 0;

	// receive file from server
	// check if the file could create
	char* filename = "E:\\MyHash.exe";
	// char* filename = "D:\\test.txt";
	FILE* fp = fopen(filename, "wb");
	if (NULL == fp)
	{
		cout << "create file failed!" << endl;
		cout << "can't transform file!" << endl;
/*		return ;*/
	}
	char* logName = "E:\\log.txt";
	FILE* flp = fopen(logName, "wb");
	if (NULL == flp)
	{
		cout << "create log file failed!" << endl;
	}

	while (TRUE)
	{
		memset(recvBuf, 0, RECVSIZE);
		nCount = recv(*sServer, recvBuf, RECVSIZE, 0);
		if (SOCKET_ERROR == nCount)
		{
			cout << "accetp data failed!" << endl;
			closesocket(*sServer);
			WSACleanup();
			return ;
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
// 			if (fileStart)
// 			{
// 				strcpy(fileSize, recvBuf+5);
// 				fileStart = false;
// 				restSize = fsize;
// 			}
			transTime++;
			strcpy(fileSize, recvBuf+5);
			// fsize = fileSize[0];
			fsize = atoi(fileSize);
			recv(*sServer, recvBuf, RECVSIZE, 0);
			fwrite(recvBuf+5, fsize > BUFSIZE ? BUFSIZE : fsize, 1, fp);
			restSize = fsize - (fsize > BUFSIZE ? BUFSIZE : fsize);
			printf("file remain %d\n", restSize);
			strcat(logData, fileSize);
			strcat(logData, "\r\n");
			fwrite(logData, strlen(logData), 1, flp);
			memset(logData+13, 0, sizeof(logData));
		}
		else if (!strcmp(bufTop, "1996e")) // transform end
		{
			cout << "receive file succeed!" << endl;
			fclose(fp);
			fclose(flp);
		}
		else if ((strcmp(bufTop, "f1996")) || (strcmp(bufTop, "1996e")) || (!strcmp(bufTop, "00000")))
		{
			// receive data from server
			cout << "receive from server:" << recvBuf << endl;
		}
		else
		{
			cout << "error" << endl;
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