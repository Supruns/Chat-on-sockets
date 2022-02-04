#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <wincon.h>
#define CONST_MES 256

void color_text_blue();
void color_text_green();
void f_exit();

main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	WSADATA ws;
	WSAStartup(MAKEWORD(2,2), (WSADATA*)&ws);

	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET) printf("\n\tThe socket was created successfully.\n\n");
	else {
		printf ("\n\tError. Failed to create socket.");
		f_exit();
	}

	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1234);

	char start;
	printf("\tChoose a chat startup option:\n - press \"S\" to start the program as a server.\n - press \"C\" to start the program as a user.\n");

	do {
		start=getch();

		if (start == 's' || start == 'S') {
			Sleep(500);
			printf("Waiting to connect to server ...\n");
			Sleep(500);

			if ((bind(s, (const struct sockaddr *)&sa, sizeof(sa)))!=SOCKET_ERROR) printf("Socket connection successfully established.\n");
			else {
				printf("Error. Failed to connect to socket.");
				f_exit();
			}
			Sleep(500);

			if ((listen(s, 5))!=SOCKET_ERROR) printf("Listening mode started successfully.\n");
			else {
				printf("Error. Failed to start listening mode.");
				f_exit();
			}

			SOCKET client_socket;
			SOCKADDR_IN client_addr;
			int client_addr_size = sizeof(client_addr);

			while(client_socket = accept(s, (struct sockaddr *)&client_addr, &client_addr_size)) {
				Sleep(500);
				printf("User connection successfully established.\n");

				char mes[CONST_MES];
				WSAEventSelect(client_socket, 0, FD_READ);
				printf("\nTo write a message, press \"Space\".\n");
				do {
					if(GetKeyState(VK_SPACE) < 0 && (GetForegroundWindow() == GetConsoleWindow()) ) {
						color_text_blue();
						printf("Enter message: ");
						gets(mes);
						send(client_socket, mes, sizeof(mes), 0);
					}
					if (recv(client_socket, mes, sizeof(mes), 0) > 0) {
						color_text_green();
						printf("User: %s\n", mes);
					}
				} while(GetKeyState(VK_ESCAPE) >= 0);

			}
		} else if (start == 'c' || start == 'C') {
			sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			Sleep(500);

			if (connect(s, (const struct sockaddr *)&sa, sizeof(sa))!=SOCKET_ERROR) printf("Connection to server successfully established.\n");
			else {
				printf("Connection to the server could not be established.");
				f_exit();
			}
			WSAEventSelect(s, 0, FD_READ);
			char mes[CONST_MES];
			printf("\nTo write a message, press \"Space\".\n");
			do {
				if(GetKeyState(VK_SPACE) < 0 && (GetForegroundWindow() == GetConsoleWindow()) ) {
					color_text_green();
					printf("Enter message: ");
					gets(mes);
					send(s, mes, sizeof(mes), 0);
				}
				if (recv(s, mes, sizeof(mes), 0) > 0) {
					color_text_blue();
					printf("Server: %s\n", mes);
				}
			} while(GetKeyState(VK_ESCAPE) >= 0);

		} else printf ("\tNot the correct choice, please try again:\n - press \"S\" to start the program as a server.\n - press \"C\" to start the program as a user.\n");
	} while(start != 'c' || start != 'C' || start != 's' || start != 'S');

	closesocket(s);
}

void color_text_blue() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void color_text_green() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
}
void f_exit() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	printf("\nThe program will shut down:\n");
	int i;
	for (i=5; i>0; i--) {
		printf("\t%d sec.\n", i);
		Sleep(1000);
	}
	exit(0);
}
