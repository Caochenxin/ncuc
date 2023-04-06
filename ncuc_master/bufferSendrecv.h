void bufferPrint(const char* buffer, size_t buffer_size);
int socketCreate(char *networkport);
int socketClose(int sock_fd);
int bufferSend(int sock_fd, char *transbuffer);
char *bufferReceive(int sock_fd, char *buffer, int timeout);