#define _WINDOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")


int main()
{
	/* WinSock�̏����� */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return 1;
	}





	WSACleanup();
	return 0;
}


