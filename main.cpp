/* 簡易チャットプログラム */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSockライブラリの指定 */

// チャットプログラム　クライアント関数
void ChatClient()
{
    SOCKET s;
    SOCKADDR_IN saddr;
    u_short uport = 8080;
    char szServer[1024] = { "192.168.3.4"};
    unsigned int addr;

    // ポート番号の表示
    printf("ポート : %d\n", uport);

    // IPアドレスの表示
    printf("IPアドレス : %s\n", szServer);

    // ソケットをオープン
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("ソケットオープンエラー\n");
        return;
    }

    // サーバーを名前で取得する
    HOSTENT* lpHost;
    lpHost = gethostbyname(szServer);
    if (lpHost == NULL) {
        /* サーバーをIPアドレスで取得する */
        addr = inet_addr(szServer);
        lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
    }

    // クライアントソケットをサーバーに接続
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(uport);
    saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);

    if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
        printf("サーバーと接続できませんでした\n");
        closesocket(s);
        return;
    }

    printf("サーバーに接続できました\n");

    while (1) {
        char sendBuf[1024] = { 0 };
        char recvBuf[1024] = { 0 };
        int nRcv;

        // 送信データの入力
        printf("送信 : ");
        scanf_s("%s", sendBuf, 1024);
        fflush(stdin);

        // データを送信
        send(s, sendBuf, (int)strlen(sendBuf), 0);

        // データを受信
        nRcv = recv(s, recvBuf, sizeof(recvBuf) - 1, 0);
        if (nRcv > 0) {
            recvBuf[nRcv] = '\0';
            printf("受信 : %s\n", recvBuf);
        } else {
            printf("サーバーからの受信に失敗しました\n");
            break;
        }
    }

    // ソケットを閉じる
    closesocket(s);
}

// チャットプログラム メイン関数
int main(void)
{
    WSADATA wsaData;

    // WinSockの初期化
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        // 初期化エラー
        printf("WinSockの初期化に失敗しました\n");
        return 1;
    }

    // クライアントとして起動
    ChatClient();

    // WinSockの終了処理
    WSACleanup();

    return 0;
}
//eof