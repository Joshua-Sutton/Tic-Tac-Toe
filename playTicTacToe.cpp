#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "TicTacToe.h"
#include <vector>
#include <string>
#pragma comment (lib, "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

int checkIfWinner(int countXs, int countOs) {
	if (countXs == 3) { return 1; }
	else if (countOs == 3) { return 2; }
	else { return 0; }
}

int checkGameStatus(vector<char> board) {
	int gameOnGoing = 0;
	int xWins = 1;
	int oWins = 2;
	int gameTie = 3;

	int countXs = 0;
	int countOs = 0;

	int gameStatus = 0;
	
	//CHECKING HORIZONTAL WINNERS
	//Spot 0 - 2
	for (int i = 0; i <= 2; i++) { if (board.at(i) == 'X') { countXs++;} else if (board.at(i) == 'O') {countOs++;}}
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1;}
	else if (gameStatus == oWins){ return 2;}
	countOs = 0;
	countXs = 0;

	//Spot 3 - 5
	for (int i = 3; i <= 5; i++) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; }}
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//Spot 6 - 8
	for (int i = 6; i <= 8; i++) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; }}
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//CHECKING VERTICAL WINNERS
	//Spot 0, 3, 6
	for (int i = 0; i <= 6; i = i + 3) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; } }
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//Spot 1, 4, 7
	for (int i = 1; i <= 7; i = i + 3) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; } }
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//Spot 2, 5, 8
	for (int i = 2; i <= 8; i = i + 3) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; } }
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//CHECKING DIAGONALS
	//Spot 0, 4, 8
	for (int i = 0; i <= 8; i = i + 4) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; } }
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	//Spot 2, 4, 6
	for (int i = 2; i <= 6; i = i + 2) { if (board.at(i) == 'X') { countXs++; } else if (board.at(i) == 'O') { countOs++; } }
	gameStatus = checkIfWinner(countXs, countOs);
	if (gameStatus == xWins) { return 1; }
	else if (gameStatus == oWins) { return 2; }
	countOs = 0;
	countXs = 0;

	return 0;
}

void displayBoard(vector<char> board) {
	cout << "     |     |     " << endl;
	cout << "  " << board.at(0) << "  |  " << board.at(1) << "  |  " << board.at(2) << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << board.at(3) << "  |  " << board.at(4) << "  |  " << board.at(5) << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << board.at(6) << "  |  " << board.at(7) << "  |  " << board.at(8) << endl;
	cout << "     |     |     " << endl;

}

int playTicTacToe(SOCKET s, char* serverName, sockaddr_in addr, int player) 
{
	//THINGS TO DO:
	// 
	// 3. !! NEED TO TEST !! Make sure we are only commuicating with opponent, no one else
	//    a. compare serverName or compare addr before placing move
	//
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	sockaddr_in playerAddr = addr;
	int addrlenINT = 100;
	int* addrlen = &addrlenINT;
	int iResult;
	int gameStatus = 0;
	int plays = 1;
	int currentPlayer = 1;
	vector<char> board = { '1', '2', '3', '4', '5', '6', '7', '8', '9', };
	int placeMarker = 0;

	displayBoard(board);
	while (gameStatus == 0) {
		if (currentPlayer == 1) {
			if (player == 1) {
				cout << "You are X's." << endl;
				cout << "Where would you like to place piece? ";
				cin.getline(sendbuf, DEFAULT_BUFLEN);
				cout << endl;
				placeMarker = atoi(sendbuf);

				while (placeMarker < 1 || placeMarker > 9 || board.at(placeMarker - 1) == 'X' || board.at(placeMarker-1) == 'O') {
					cout << "Invalid Location... try again " << endl;
					displayBoard(board);
					cout << "Where would you like to place piece? ";
					cin.getline(sendbuf, DEFAULT_BUFLEN);
					placeMarker = atoi(sendbuf);
				}

				board.at(placeMarker - 1) = 'X';

				iResult = sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, sizeof(addr));
				if (iResult == SOCKET_ERROR) {
					cout << "send failed: " << WSAGetLastError() << endl;
					closesocket(s);
					WSACleanup();
					return 1;
				}

				displayBoard(board);
			}
			else if (player == 2) {
				cout << "... waiting for opponet ..." << endl;
				int waitCode = wait(s, 20, 0);

				if (waitCode == 1) {
					iResult = recvfrom(s, recvbuf, (int)strlen(recvbuf) + 512, 0, (sockaddr*)&addr, addrlen);
					if (iResult > 0) {
						if (addr.sin_addr.S_un.S_addr == playerAddr.sin_addr.S_un.S_addr) {
							placeMarker = atoi(recvbuf);
						}
						
					}
				}
				else if (waitCode == 0) {
					cout << "YOU WIN! Other player took to long..." << endl;
					return ABORT;
				}

				if (placeMarker < 1 || placeMarker > 9 || board.at(placeMarker - 1) == 'X' || board.at(placeMarker - 1) == 'O') {
					cout << "YOU WIN! Invalid Move By Other Player..." << endl;
					return oWinner;
				}

				board.at(placeMarker - 1) = 'X';
				displayBoard(board);
				cout << endl;
			}

			gameStatus = checkGameStatus(board);
			currentPlayer = 2;
			plays++;
		}
		else if (currentPlayer == 2) {
			if (player == 2) {
				cout << "You are O's" << endl;
				cout << "Where would you like to place piece? ";
				cin.getline(sendbuf, DEFAULT_BUFLEN);
				cout << endl;
				placeMarker = atoi(sendbuf);

				while (placeMarker < 1 || placeMarker > 9 || board.at(placeMarker - 1) == 'X' || board.at(placeMarker - 1) == 'O') {
					cout << "Invalid Location... try again " << endl;
					displayBoard(board);
					cout << "Where would you like to place piece? ";
					cin.getline(sendbuf, DEFAULT_BUFLEN);
					placeMarker = atoi(sendbuf);
				}

				board.at(placeMarker - 1) = 'O';
				displayBoard(board);
				cout << endl;

				iResult = sendto(s, sendbuf, (int)strlen(sendbuf) + 1, 0, (sockaddr*)&addr, sizeof(addr));
				if (iResult == SOCKET_ERROR) {
					cout << "send failed: " << WSAGetLastError() << endl;
					closesocket(s);
					WSACleanup();
					return 1;
				}
			}
			else if (player == 1) {
				cout << "... waiting for opponet ..." << endl;
				cout << endl;
				int waitCode = wait(s, 20, 0);

				if (waitCode == 1) {
					if(serverName)
					iResult = recvfrom(s, recvbuf, (int)strlen(recvbuf) + 512, 0, (sockaddr*)&addr, addrlen);
					if (iResult > 0) {
						placeMarker = atoi(recvbuf);
					}
				}
				else if (waitCode == 0) {
					cout << "YOU WIN! Other player took to long..." << endl;
					return ABORT;
				}

				if (placeMarker < 1 || placeMarker > 9 || board.at(placeMarker - 1) == 'X' || board.at(placeMarker - 1) == 'O') {
					cout << "YOU WIN! Invalid Move By Other Player..." << endl;
					return xWinner;
				}

				board.at(placeMarker - 1) = 'O';
				displayBoard(board);
				cout << endl;
			}

			gameStatus = checkGameStatus(board);
			currentPlayer = 1;
			plays++;
		}

		if (plays == 10) { gameStatus = 3; }
	}

	if (gameStatus == 1) {
		if (player == 1) {
			cout << "TIC TAC TOE! YOU WIN!" << endl;
		}
		else if (player == 2) {
			cout << "You Lose ..." << endl;
		}
		return xWinner;
	}
	else if (gameStatus == 2) {
		if (player == 1) {
			cout << "You Lose ..." << endl;
			
		}
		else if (player == 2) {
			cout << "TIC TAC TOE! YOU WIN!" << endl;
		}
		return oWinner;
	}
	else if (gameStatus == 3) {
		if (player == 1) {
			cout << "Tie .. no winner ... BOO" << endl;

		}
		else if (player == 2) {
			cout << "Tie .. no winner ... BOO" << endl;
		}
		return TIE;
	}

}