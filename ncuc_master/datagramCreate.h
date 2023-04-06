char * read_register(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len);
char * write_register(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len,unsigned char* data);
char * read_channel(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len);
char * write_channel(unsigned char ncuc_index,unsigned short ncuc_ADDR,unsigned short ncuc_len,unsigned char* data);