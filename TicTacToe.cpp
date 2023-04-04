#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "TicTacToe.h"
#pragma comment (lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
using std::cout;
using std::cin;
using std::endl;
int main(int argc, char* argv[])
{
	// 1. Initialize Winsock (both client and server)
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << '\n';
		return 1;
	}
	char playerName[MAX_NAME], choice[2], IPAddress[20] = "", subnetMask[20] =
		"";
	cout << "What is your name? ";
	cin.getline(playerName, MAX_NAME);
	do {
		cout << endl;
		cout << "Choose an option:" << endl;
		cout << " 1 - Host a game of Tic-Tac-Toe" << endl;
		cout << " 2 - Challenge someone else" << endl;
		cout << " 3 - Quit" << endl;
		cout << "Enter 1, 2 or 3: ";
		cin.getline(choice, 2);
		switch (choice[0]) {
		case '1': server_main(playerName); break;
		case '2': client_main(playerName, IPAddress, subnetMask); break;
		case '3': break;
		default: cout << endl << "Please enter a digit between 1 and 3." <<
			endl;
		}
	} while (choice[0] != '3');
	WSACleanup();
	return 0;
}
int client_main(char* playerName, char* IPAddress, char* subnetMask)
{
	int iResult;
	ServerStruct server[MAX_SERVERS];
	int numServers;
	SOCKET GameSocket = INVALID_SOCKET;
	GameSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (GameSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}
	// Necessary to be able to send a broadcast message
	BOOL bOptVal = TRUE;
	setsockopt(GameSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal,
		sizeof(BOOL));
	cout << "\nLooking for Tic Tac Toe servers...\n";
	getServers(GameSocket, GetBroadcastAddress(IPAddress, subnetMask), server,
		numServers);
	if (numServers == 0) cout << "No servers found. Try again later.\n\n";
	else {
		cout << "Found Tic Tac Toe server";
		if (numServers == 1) cout << ": " << server[0].name << endl;
		else {
			cout << "s:\n";
			for (int i = 0; i < numServers; i++)
				cout << " " << i + 1 << " - " << server[i].name << endl;
			cout << endl << " " << numServers + 1 << " - QUIT\n";
		}
		cout << endl;
		int answer = 0;
		char answerstr[MAX_NAME];
		if (numServers == 1) {
			cout << "Do you want to challenge " << server[0].name << "? ";
			cin.getline(answerstr, MAX_NAME);
			if (_stricmp(answerstr, "y") == 0) answer = 1;
		}
		else if (numServers > 1) {
			cout << "Who would you like to challenge (1-" << numServers + 1
				<< ")? ";
			cin.getline(answerstr, MAX_NAME);
			answer = atoi(answerstr);
			if (answer > numServers) answer = 0;
		}
		if (answer >= 1 && answer <= numServers) {
			char serverName[MAX_NAME];
			strcpy_s(serverName, server[answer - 1].name);
			char buf[DEFAULT_BUFLEN];
			strcpy_s(buf, TicTacToe_CHALLENGE);
			strcat_s(buf, playerName);
			iResult = sendto(GameSocket, buf, strlen(buf) + 1, 0,
				(sockaddr*)&server[answer - 1].addr, sizeof(server[answer - 1].addr));
			int winner = playTicTacToe(GameSocket, serverName, server[answer- 1].addr, X_PLAYER);
		}
	}
	closesocket(GameSocket);
	return 0;
}
int server_main(char* playerName)
{
	int iResult;
	SOCKET GameSocket = INVALID_SOCKET;
	GameSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (GameSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}
	// Data in the sin_port and sin_addr members of the sockaddr_in struct
	// need to be in network byte order. Use htons to convert the port
	// number and htonl to convert the IP address. Port numbers and addresses
	// returned by getaddrinfo are already in network byte order.
	struct sockaddr_in myAddr;
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(DEFAULT_PORT);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	iResult = bind(GameSocket, (SOCKADDR*)&myAddr, sizeof(myAddr));
	if (iResult == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << '\n';
		closesocket(GameSocket);
		WSACleanup();
		return 1;
	}
	// Necessary to be able to receive a broadcast message
	BOOL bOptVal = TRUE;
	setsockopt(GameSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal,
		sizeof(BOOL));
	char recvbuf[DEFAULT_BUFLEN], sendbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	struct sockaddr_in addr;
	int addrSize = sizeof(addr);
	cout << "\nWaiting for a challenge...\n";
	// Since our only job is to sit around for client queries, there is no need to call wait
		iResult = recvfrom(GameSocket, recvbuf, recvbuflen, 0, (sockaddr*)&addr,
			&addrSize);
	cout << "\nReceived: " << recvbuf << endl;
	bool finished = false;
	while (!finished) {
		if (strcmp(recvbuf, TicTacToe_QUERY) == 0) {
			strcpy_s(sendbuf, TicTacToe_NAME);
			strcat_s(sendbuf, playerName);
			sendto(GameSocket, sendbuf, strlen(sendbuf) + 1, 0,
				(sockaddr*)&addr, addrSize);
			cout << "Sent: " << sendbuf << endl;
		}
		else if (strncmp(recvbuf, TicTacToe_CHALLENGE,
			strlen(TicTacToe_CHALLENGE)) == 0) {
			char* startOfName = strstr(recvbuf, TicTacToe_CHALLENGE);
			if (startOfName != NULL) {
				cout << endl << "You have been challenged by " <<
					startOfName + strlen(TicTacToe_CHALLENGE) << endl;
			}
			int winner = playTicTacToe(GameSocket, playerName, addr,
				O_PLAYER);
			finished = true;
		}
		if (!finished) {
			iResult = recvfrom(GameSocket, recvbuf, recvbuflen, 0,
				(sockaddr*)&addr, &addrSize);
			cout << "\nReceived: " << recvbuf << endl;
		}
	}
	closesocket(GameSocket);
	return 0;
}
