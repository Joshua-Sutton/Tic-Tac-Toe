#pragma once
#define MAX_NAME 50
#define DEFAULT_PORT 28900
#define TicTacToe_QUERY "Who?"
#define TicTacToe_NAME "Name="
#define TicTacToe_CHALLENGE "Player="
#define DEFAULT_BUFLEN 512
#define MAX_SERVERS 100
const int X_PLAYER = 1;
const int O_PLAYER = 2;
const int ABORT = -1;
const int noWinner = 0;
const int xWinner = 1;
const int oWinner = 2;
const int TIE = 3;
struct ServerStruct {
	char name[MAX_NAME];
	sockaddr_in addr;
};
int wait(SOCKET s, int seconds, int msec);
sockaddr_in GetBroadcastAddress(char* IPAddress, char* subnetMask);
void getServers(SOCKET s, sockaddr_in broadcastAddress, ServerStruct server[], int&
	numServers);
int client_main(char* playerName, char* IPAddress, char* subnetMask);
int server_main(char* playerName);
int playTicTacToe(SOCKET s, char* serverName, sockaddr_in addr, int player);
/*****************
* Board Format *
* *
* 1 | 2 | 3 *
* ----------- *
* 4 | 5 | 6 *
* ----------- *
* 7 | 8 | 9 *
* *
*****************/
