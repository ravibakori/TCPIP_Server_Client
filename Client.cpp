// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib,"ws2_32.lib")

using namespace std;


void main()
{
	string ipAddress = "127.0.0.1";
	int port = 54000;
	//initialize Winsocket
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	
	int WSResult = WSAStartup(ver, &data);
	if (WSResult != 0)
	{
		cerr << "cant start winsock error#"<< WSResult << endl;
		return;
	}
	
	//Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "cant create socket error#" << sock << endl;
		return;
	}
		
	//fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//connect to server
	int ConnResult = connect(sock, (SOCKADDR*)&hint, sizeof(hint));
	if (ConnResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server" << endl;
		closesocket(sock);
		WSACleanup;
		return;
	}

	//Do-while loop to send 
	char buffer[4096];
	string userInput;

	do
	{
		//take string from user
		cout << "RavI>";
		getline(cin,userInput);
		
		if (userInput != "exit")
		{
			// send text to server
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				int byteReceive = recv(sock, buffer, 4096, 0);
				if (byteReceive > 0)
				{
				
					//echo response to consol
					cout << "server > " << string(buffer, 0, byteReceive) << endl;
				}
							
			}
		}

	} while (userInput != "exit");



	//Gracefully close down everything
	closesocket(sock);
}

