#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"datagramCreate.h"

// 定义以太网帧头部结构体
typedef struct {
    unsigned char h_dest[6];   // 目标MAC地址
    unsigned char h_source[6];    // 源MAC地址
    unsigned short h_proto;         // 以太网类型
}__attribute__ ((packed)) ethernet_header;

// 定义NCUC2.0 Header结构体
typedef struct {
    unsigned char index;         // 8bit的index
    unsigned char version_r_cycle_cf ; // 4bit的version -- 2bit的R -- 1bit的cycle -- 1bit的circulating flag
}__attribute__ ((packed)) ncuc20_header;

// 定义NCUC2.0 Datagram结构体
typedef struct {
    unsigned char cmd_m ;       // 7bit的命令位 -- 1bit的M标志位
    unsigned short ST;          // 16bit的ST段
    unsigned short ADDR;        // 16bit的ADDR段
    unsigned short LEN_R ;        // 11bit的LEN段 -- 5bit的R      
}__attribute__ ((packed)) ncuc20_datagram;
// 测试程序
char * getbuffer(char* buffer, unsigned char ncuc_index,unsigned char ncuc_cmd,unsigned short ncuc_ADDR,unsigned short ncuc_len,unsigned char* data)
{

unsigned char ncuc_h_dest[6]={0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
unsigned char ncuc_h_source[6]={0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
unsigned short nucu_h_proto=0x8888;

//unsigned char ncuc_index=0;
unsigned char ncuc_version_r_cycle_cf= 0b0010 | 0b00<<4 | 0b0<<6 | 0b0<<7; // 4bit的version -- 2bit的R -- 1bit的cycle -- 1bit的circulating flag
//unsigned char ncuc_version_r_cycle_cf= 0b0010<<4 | 0b00<<2 | 0b1<<1 | 0b1; // 4bit的version -- 2bit的R -- 1bit的cycle -- 1bit的circulating flag

unsigned char ncuc_cmd_m = ncuc_cmd | 0b0<<7 ;// 7bit的命令位 -- 1bit的M标志位
unsigned short ncuc_ST=0x0000;          
//unsigned short ncuc_ADDR=0x0060;  
unsigned short  ncuc_LEN_R= ncuc_len | 0b10<<11 ;// 11bit的LEN段 -- 5bit的R  
//unsigned short  ncuc_LEN_R = 0x02<< 5 | 0b0;// 11bit的LEN段 -- 5bit的R  

//unsigned char data[16]={0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd};

unsigned short  ncuc_O_F_IRQ  = 0b0<<15 | 0b0<<14 | 0x0; //1个bit的O段 -- 1个bit的F -- 14bit的IRQ
//unsigned short  ncuc_O_F_IRQ  = 0b0<<15 | 0b0<<14 | 0x2; //1个bit的O段 -- 1个bit的F -- 14bit的IRQ

int offset=0;
ethernet_header  *eth_hdr;
eth_hdr = (ethernet_header *)buffer;
eth_hdr->h_dest[0] = ncuc_h_dest[0];
eth_hdr->h_dest[1] = ncuc_h_dest[1];
eth_hdr->h_dest[2] = ncuc_h_dest[2];
eth_hdr->h_dest[3] = ncuc_h_dest[3];
eth_hdr->h_dest[4] = ncuc_h_dest[4];
eth_hdr->h_dest[5] = ncuc_h_dest[5];
eth_hdr->h_source[0] = ncuc_h_source[0];
eth_hdr->h_source[1] = ncuc_h_source[1];;
eth_hdr->h_source[2] = ncuc_h_source[2];;
eth_hdr->h_source[3] = ncuc_h_source[3];;
eth_hdr->h_source[4] = ncuc_h_source[4];;
eth_hdr->h_source[5] = ncuc_h_source[5];;
eth_hdr->h_proto = nucu_h_proto;
offset=offset+sizeof(ethernet_header);
//printf("%d\n",offset);
ncuc20_header * ncuc20_hd;
ncuc20_hd=(ncuc20_header *)(buffer+offset);
ncuc20_hd->index=ncuc_index;
ncuc20_hd->version_r_cycle_cf=ncuc_version_r_cycle_cf;
offset=offset+sizeof(ncuc20_header);

//printf("%d\n",offset);

ncuc20_datagram *ncuc20_dgram;
ncuc20_dgram=(ncuc20_datagram *)(buffer+offset);
ncuc20_dgram->cmd_m=ncuc_cmd_m;
ncuc20_dgram->ST =ncuc_ST;
ncuc20_dgram->ADDR = ncuc_ADDR;
ncuc20_dgram->LEN_R=ncuc_LEN_R;

offset=offset+sizeof(ncuc20_datagram);
//printf("%d\n",offset);
memcpy(buffer+offset,data,ncuc_len);
offset=offset+ncuc_len;

//printf("%d\n",offset);
unsigned short * O_F_IRQ;
O_F_IRQ=(unsigned short *)(buffer+offset);
*O_F_IRQ=ncuc_O_F_IRQ;

//print_buffer(buffer, 64);
//todo

return buffer;
}





char * read_register(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len)
{
    unsigned char ncuc_cmd=0x10;

    unsigned char* data = (unsigned char*)malloc(ncuc_len * sizeof(unsigned char)); // 动态分配数组空间



    memset(data, 0xdd, ncuc_len); // 将数组所有元素初始化为 0xdd

    int ncuc_framesize=29+ncuc_len;

    if (ncuc_framesize<64)
    {
        ncuc_framesize=64;
    }
    
   char* buffer = (char*)malloc(ncuc_framesize * sizeof(char)); // 动态分配数组空间
    memset(buffer, 0x0, ncuc_framesize);
   getbuffer(buffer,ncuc_index,ncuc_cmd,ncuc_ADDR,ncuc_len,data);
   return buffer;
}

char * write_register(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len,unsigned char* data)
{

   // printf("%d\n",sizeof(*data));
    if(sizeof(*data)>ncuc_len)
    {
        printf("write_register error: data size wrong");
    }
    
    unsigned char ncuc_cmd=0x11;

    int ncuc_framesize=29+ncuc_len;

    if (ncuc_framesize<64)
    {
        ncuc_framesize=64;
    }
    
   char* buffer = (char*)malloc(ncuc_framesize * sizeof(char)); // 动态分配数组空间
    memset(buffer, 0x0, ncuc_framesize);
   getbuffer(buffer,ncuc_index,ncuc_cmd,ncuc_ADDR,ncuc_len,data);
   return buffer;

}


char * read_channel(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len)
{
    unsigned char ncuc_cmd=0x20;

    unsigned char* data = (unsigned char*)malloc(ncuc_len * sizeof(unsigned char)); // 动态分配数组空间



    memset(data, 0xdd, ncuc_len); // 将数组所有元素初始化为 0xdd

    int ncuc_framesize=29+ncuc_len;

    if (ncuc_framesize<64)
    {
        ncuc_framesize=64;
    }
    
   char* buffer = (char*)malloc(ncuc_framesize * sizeof(char)); // 动态分配数组空间
    memset(buffer, 0x0, ncuc_framesize);
   getbuffer(buffer,ncuc_index,ncuc_cmd,ncuc_ADDR,ncuc_len,data);
   return buffer;
}

char * write_channel(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len,unsigned char* data)
{

   // printf("%d\n",sizeof(*data));
    if(sizeof(*data)>ncuc_len)
    {
        printf("write_register error: data size wrong");
    }
    
    unsigned char ncuc_cmd=0x21;

    int ncuc_framesize=29+ncuc_len;

    if (ncuc_framesize<64)
    {
        ncuc_framesize=64;
    }
    
   char* buffer = (char*)malloc(ncuc_framesize * sizeof(char)); // 动态分配数组空间
    memset(buffer, 0x0, ncuc_framesize);
   getbuffer(buffer,ncuc_index,ncuc_cmd,ncuc_ADDR,ncuc_len,data);
   return buffer;

}
