// Server.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	// Initialize Winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &data);
	if (wsOk != 0)
	{
		cerr << "Can't start winsock! Tactical Retreat!" << endl;
		return;
	}

	// Create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Couldn't create socket!" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // or inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];  // Client's remote name
	char service[NI_MAXHOST]; // Service (i.e port)

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port" <<
			htons(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	char bufout[10];
	int iCount = 0;
	char c = static_cast<char>(iCount);
	while (true)
	{
		ZeroMemory(buf, 4096);
		ZeroMemory(bufout, 10);

		// Wait for client to send data
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		cout << "Req:" << buf << endl;

		if (bytesRecieved == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesRecieved == 0)
		{
			cout << "client disconnected " << endl;
			break;
		}
		if (buf[0] == '\r')
			memcpy(bufout, "\n\r", 2);
		memcpy(bufout + 2, "RAVI>", 5);
	//	cout << "Res:" << bufout << endl;
	//	send(clientSocket, bufout, 7, 0);
		

		/*			if (buf[0] != '\r')
					{
						iCount++;
						memcpy(bufout + 5, buf, bytesRecieved);
						memcpy(bufout + bytesRecieved , "\r\n", 2);
						bytesRecieved = bytesRecieved + 2;
					}
				// Echo message back to client

		*/
		
		if (buf[0] != '\r')
		{
			cout.width(37);
			cout << "Res:" << buf << endl;
			
			send(clientSocket, buf, bytesRecieved, 0);
		}
	}

	// Close
	closesocket(clientSocket);

	// Cleanup Winsock
	WSACleanup();
}