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
    SOCKET s;
    SOCKADDR_IN saddr;
    u_short uport = 8080;
    char szServer[1024] = { "192.168.3.4"};
    unsigned int addr;

    // �|�[�g�ԍ��̕\��
    printf("�|�[�g : %d\n", uport);

    // IP�A�h���X�̕\��
    printf("IP�A�h���X : %s\n", szServer);

    // �\�P�b�g���I�[�v��
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("�\�P�b�g�I�[�v���G���[\n");
        return;
    }

    // �T�[�o�[�𖼑O�Ŏ擾����
    HOSTENT* lpHost;
    lpHost = gethostbyname(szServer);
    if (lpHost == NULL) {
        /* �T�[�o�[��IP�A�h���X�Ŏ擾���� */
        addr = inet_addr(szServer);
        lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
    }

    // �N���C�A���g�\�P�b�g���T�[�o�[�ɐڑ�
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(uport);
    saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);

    if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
        printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
        closesocket(s);
        return;
    }

    printf("�T�[�o�[�ɐڑ��ł��܂���\n");

    while (1) {
        char sendBuf[1024] = { 0 };
        char recvBuf[1024] = { 0 };
        int nRcv;

        // ���M�f�[�^�̓���
        printf("���M : ");
        scanf_s("%s", sendBuf, 1024);
        fflush(stdin);

        // �f�[�^�𑗐M
        send(s, sendBuf, (int)strlen(sendBuf), 0);

        // �f�[�^����M
        nRcv = recv(s, recvBuf, sizeof(recvBuf) - 1, 0);
        if (nRcv > 0) {
            recvBuf[nRcv] = '\0';
            printf("��M : %s\n", recvBuf);
        } else {
            printf("�T�[�o�[����̎�M�Ɏ��s���܂���\n");
            break;
        }
    }

    // �\�P�b�g�����
    closesocket(s);
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