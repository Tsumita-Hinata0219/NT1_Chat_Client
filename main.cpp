#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#pragma comment(lib, "WSock32.lib")
#pragma comment(lib, "Ws2_32.lib")


// ソケットを作成する関数
static SOCKET CreateSocket()
{
	// ソケットの作成
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket == INVALID_SOCKET) {
		printf("ソケット作成に失敗しました。エラーコード: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	printf("socket 成功\n");
	return mySocket;
}

// サーバーのホスト情報を取得
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

// クラインとソケットをサーバーに接続
static void ConnectToServer(SOCKET sockfd, const char* ipAddress, int portNumber) {
	sockaddr_in server_addr{};
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);

	if (connect(sockfd, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("サーバーと接続できませんでした\n");
		closesocket(sockfd);
	}
	else {
		printf("サーバーと接続しました\n");
	}
}



int main()
{
	/* WinSockの初期化 */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartupに失敗しました。エラーコード: %d\n", WSAGetLastError());
		return 1;
	}

	// ポートは固定
	unsigned short portNumber = 8080;
	printf("ポート : %d\n", portNumber);
	// IPアドレス
	char IPAddr[1024] = { "192.168.137.1" };
	printf("IPアドレス : %s\n", IPAddr);

	/* ソケット作成 */
	SOCKET sockfd = CreateSocket();

	/* サーバーを名前で取得する */
	HOSTENT* lpHost = GetHostByNameOrAddress(IPAddr);

	/* クラインとソケットをサーバーに接続 */
	ConnectToServer(sockfd, IPAddr, portNumber);


	// サーバー側との文字列のやりとり
	while (1) {

		int nRcv = 0;
		char szBuf[1024]{};

		printf("送信 : ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sockfd, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(sockfd, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("受信 : %s\n", szBuf);
	}


	closesocket(sockfd);
	WSACleanup();
	return 0;
}

