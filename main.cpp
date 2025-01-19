#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#pragma comment(lib, "WSock32.lib")
#pragma comment(lib, "Ws2_32.lib")


// �\�P�b�g���쐬����֐�
static SOCKET CreateSocket()
{
	// �\�P�b�g�̍쐬
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == INVALID_SOCKET) {
		printf("�\�P�b�g�쐬�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	printf("socket ����\n");
	return mySocket;
}

// �T�[�o�[�̃z�X�g�����擾
HOSTENT* GetHostByNameOrAddress(const char* ipAddress) {
	HOSTENT* lpHost{};
	unsigned int addr;

	lpHost = gethostbyname(ipAddress);
	if (lpHost == NULL) {
		addr = inet_addr(ipAddress);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	return lpHost;
}

// �N���C���ƃ\�P�b�g���T�[�o�[�ɐڑ�
static void ConnectToServer(SOCKET sockfd, const char* ipAddress, int portNumber) {
	sockaddr_in server_addr{};
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);

	if (connect(sockfd, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
		closesocket(sockfd);
	}
	else {
		printf("�T�[�o�[�Ɛڑ����܂���\n");
	}
}



int main()
{
	/* WinSock�̏����� */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup�Ɏ��s���܂����B�G���[�R�[�h: %d\n", WSAGetLastError());
		return 1;
	}

	// �|�[�g�͌Œ�
	unsigned short portNumber = 8080;
	printf("�|�[�g : %d\n", portNumber);
	// IP�A�h���X
	char IPAddr[1024] = { "192.168.137.1" };
	printf("IP�A�h���X : %s\n", IPAddr);

	/* �\�P�b�g�쐬 */
	SOCKET sockfd = CreateSocket();

	/* �T�[�o�[�𖼑O�Ŏ擾���� */
	HOSTENT* lpHost = GetHostByNameOrAddress(IPAddr);

	/* �N���C���ƃ\�P�b�g���T�[�o�[�ɐڑ� */
	ConnectToServer(sockfd, IPAddr, portNumber);


	// �T�[�o�[���Ƃ̕�����̂��Ƃ�
	while (1) {

		int nRcv = 0;
		char szBuf[1024]{};

		printf("���M : ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sockfd, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(sockfd, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("��M : %s\n", szBuf);
	}


	closesocket(sockfd);
	WSACleanup();
	return 0;
}

