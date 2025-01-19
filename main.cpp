#define _WINDOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

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
	HOSTENT* ipHost{};
	ipHost = gethostbyname(IPAddr);

	/* クラインとソケットをサーバーに接続 */
	ConnectToServer(sockfd, IPAddr, portNumber);


	closesocket(sockfd);
	WSACleanup();
	return 0;
}

