#define _WINDOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")


int main()
{
	/* WinSockの初期化 */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartupに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return 1;
	}





	WSACleanup();
	return 0;
}


