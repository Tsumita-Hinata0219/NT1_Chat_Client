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
    int recv_cnt;
    int send_cnt;
    int fromlen;
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0); // 修正: int から SOCKET に変更
    struct sockaddr_in send_addr, recv_addr;
    char szBuf[512];

    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(8000);
    send_addr.sin_addr.S_un.S_addr = inet_addr("192.168.3.4"); // 送信アドレスを設定

    while (1)
    {
        recv_cnt = 0;
        send_cnt = 0;
        memset(szBuf, 0, sizeof(szBuf));
        fromlen = sizeof(recv_addr);

        //送信メッセージ入力
        printf("送信 --> ");
        scanf_s("%511s", szBuf, (unsigned int)sizeof(szBuf)); // 修正: フォーマット指定子と引数の型を修正
        fflush(stdin);

        //データ送信
        while (send_cnt == 0)
        {
            send_cnt = sendto(sock, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&send_addr, sizeof(send_addr));
        }

        //データ受信
        puts("受信待ち…");
        memset(szBuf, 0, sizeof(szBuf));
        recv_cnt = recvfrom(sock, szBuf, sizeof(szBuf) - 1, 0, (struct sockaddr*)&recv_addr, &fromlen);
        printf("受信 : %s \n", szBuf);
    }

    closesocket(sock);
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