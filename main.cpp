/* �ȈՃ`���b�g�v���O���� */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h> /* WinSock�̃w�b�_�t�@�C�� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSock���C�u�����̎w�� */

// �`���b�g�v���O�����@�N���C�A���g�֐�
void ChatClient()
{
    int recv_cnt;
    int send_cnt;
    int fromlen;
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0); // �C��: int ���� SOCKET �ɕύX
    struct sockaddr_in send_addr, recv_addr;
    char szBuf[512];

    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(8000);
    send_addr.sin_addr.S_un.S_addr = inet_addr("192.168.3.4"); // ���M�A�h���X��ݒ�

    while (1)
    {
        recv_cnt = 0;
        send_cnt = 0;
        memset(szBuf, 0, sizeof(szBuf));
        fromlen = sizeof(recv_addr);

        //���M���b�Z�[�W����
        printf("���M --> ");
        scanf_s("%511s", szBuf, (unsigned int)sizeof(szBuf)); // �C��: �t�H�[�}�b�g�w��q�ƈ����̌^���C��
        fflush(stdin);

        //�f�[�^���M
        while (send_cnt == 0)
        {
            send_cnt = sendto(sock, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&send_addr, sizeof(send_addr));
        }

        //�f�[�^��M
        puts("��M�҂��c");
        memset(szBuf, 0, sizeof(szBuf));
        recv_cnt = recvfrom(sock, szBuf, sizeof(szBuf) - 1, 0, (struct sockaddr*)&recv_addr, &fromlen);
        printf("��M : %s \n", szBuf);
    }

    closesocket(sock);
}

// �`���b�g�v���O���� ���C���֐�
int main(void)
{
    WSADATA wsaData;

    // WinSock�̏�����
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        // �������G���[
        printf("WinSock�̏������Ɏ��s���܂���\n");
        return 1;
    }

    // �N���C�A���g�Ƃ��ċN��
    ChatClient();

    // WinSock�̏I������
    WSACleanup();

    return 0;
}
//eof