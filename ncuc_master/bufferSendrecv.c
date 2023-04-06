#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include "bufferSendrecv.h"

// 接收缓冲区大小
#define BUF_SIZE 60

// 打印报文
void bufferPrint(const char *buffer, size_t buffer_size)
{
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

// 创建原始套接字
int socketCreate(char *networkport)
{
    int sock_fd;
    struct sockaddr_ll sll;
    struct ifreq ifr;

    // 创建套接字
    sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置接口名称
    strncpy(ifr.ifr_name, networkport, IFNAMSIZ);

    // 获取接口索引
    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) == -1)
    {
        perror("ioctl");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 设置原始套接字地址结构
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_ALL);

    // 绑定原始套接字
    if (bind(sock_fd, (struct sockaddr *)&sll, sizeof(sll)) == -1)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}

// 关闭原始套接字
int socketClose(int sock_fd)
{
    close(sock_fd);
    return 0;
}

// 发送报文
int bufferSend(int sock_fd, char *transbuffer)
{
    char buffer[BUF_SIZE];
    memcpy(buffer, transbuffer, 60);
    /*
     for(int i=26;i<60;i++)
     {
         buffer[i]=0xcc;
     }
     */
    printf("Send:");
    bufferPrint(buffer, sizeof(buffer));

    // 发送报文
    if (send(sock_fd, buffer, sizeof(buffer), 0) == -1)
    {
        perror("send");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 关闭套接字
    // close(sock_fd);

    return 0;
}

// 接收报文
char *bufferReceive(int sock_fd, char *buffer, int timeout)
{
    int n;

    // 设置超时时间
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    // 设置文件描述符
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock_fd, &readfds);

    while (1)
    {
        int rv = select(sock_fd + 1, &readfds, NULL, NULL, &tv);
        if (rv == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        else if (rv == 0)
        {
            printf("Timeout!\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            n = recv(sock_fd, buffer, BUF_SIZE, 0);
            if (n == -1)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            if (memcmp(buffer, "\x11\x11\x11\x11\x11\x11\x22\x22\x22\x22\x22\x22\x88\x88", 14) == 0)
            {
                printf("Rece:");
                bufferPrint(buffer, 60);
                return buffer;
            }
        }
    }
}