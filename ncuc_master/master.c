#include <stdio.h>
#include "bufferSendrecv.h"
#include "datagramCreate.h"
// #include<time.h>

int main(int argc, char *argv[])
{
    // 设置缓冲区
    char *transbuffer;

    // 设置网口
    char *networkport1 = "enp1s0";
    char *networkport2 = "enp2s0";

    // 创建套接字
    int sock_fd1 = socketCreate(networkport1);
    int sock_fd2 = socketCreate(networkport2);

    // //01写寄存器
    // unsigned char data[4]={0x08,0x00,0x00,0x06};
    // transbuffer=write_register(1,0x120,4,data);
    // printf("Write:\n");
    // buffersender(sock_fd,transbuffer);

    // //02读寄存器
    // transbuffer=read_register(0,0x100,6);
    // printf("read:\n");
    // buffersender(sock_fd,transbuffer);

    // //03通道0配置 CH=0b
    // unsigned char data[6]={0x00,0x08,0x06,0x00,0x00,0x80};//两字节需要互换
    // transbuffer=write_register(0,0x100,6,data);
    // printf("Write:\n");
    // buffersender(sock_fd,transbuffer);

    // 01通道2配置主站可写 CH=10b
    unsigned char data1[6] = {0x00, 0x08, 0x04, 0x00, 0x00, 0x80}; // 两字节需要互换
    transbuffer = write_register(1, 0x120, 6, data1);
    printf("Write:\n");
    bufferSend(sock_fd1, transbuffer);

    // 02通道信息
    transbuffer = read_register(3, 0x120, 12);
    printf("read:\n");
    bufferSend(sock_fd1, transbuffer);

    // 03写通道
    unsigned char data2[4] = {0x56, 0x75, 0x43, 0x89};
    transbuffer = write_channel(24, 0x0000, 4, data2);
    printf("Write:\n");
    bufferSend(sock_fd1, transbuffer);

    // 04通道信息
    transbuffer = read_register(5, 0x120, 12);
    printf("read:\n");
    bufferSend(sock_fd1, transbuffer);

    // 05读通道
    transbuffer = read_channel(6, 0x0000, 1);
    printf("read:\n");
    bufferSend(sock_fd1, transbuffer);

    // 03写通道
    unsigned char data3[4] = {0x11, 0x65, 0x71, 0x89};
    transbuffer = write_channel(4, 0x0000, 4, data3);
    printf("Write:\n");
    bufferSend(sock_fd1, transbuffer);

// //06通道2关闭
// unsigned char data3[1]={0x00};//两字节需要互换
// transbuffer=write_register(7,0x125,1,data3);
// printf("Write:\n");
// buffersender(sock_fd,transbuffer);
// sleep(2);

// //07通道2配置主站可读 CH=10b
// unsigned char data4[6]={0x00,0x08,0x0c,0x00,0x10,0x80};//两字节需要互换
// transbuffer=write_register(8,0x120,6,data4);
// printf("Write:\n");
// buffersender(sock_fd,transbuffer);
// sleep(2);

// //08通道2开启
// unsigned char data5[4]={0x80};//两字节需要互换
// transbuffer=write_register(9,0x125,1,data5);
// printf("Write:\n");
// buffersender(sock_fd,transbuffer);
// sleep(2);

// //09通道信息
// transbuffer=read_register(10,0x120,12);
// printf("read:\n");
// buffersender(sock_fd,transbuffer);
// sleep(2);

// //10读通道
// transbuffer=read_channel(11,0x0000,4);
// printf("read:\n");
// buffersender(sock_fd,transbuffer);
// sleep(2);

// clock_t start=clock();
// 回收报文
#define BUF_SIZE 60 // 接收缓冲区大小
    char buffer[BUF_SIZE];
    char *bufferresult = bufferReceive(sock_fd2, buffer, 1);
    // clock_t end=clock();
    // double elapsed_time=(double)(end-start)/CLOCKS_PER_SEC;
    // printf("%.41f seconds",elapsed_time);

    // 关闭套接字
    socketClose(sock_fd1);
    socketClose(sock_fd2);
}
