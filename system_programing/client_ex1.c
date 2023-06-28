/*UDP CLIENT*/

#include "utils.h"
#include "tcp_udp_ex1.c"

int ClientDriver(void); 

int main(void)
{
    return ClientDriver(); 
}   

int ClientDriver(void)
{
    int ret = 1;
    int socket_fd = 0; 
    char message_buffer[BUFF_SIZE] = {0};
    socklen_t add_len = BUFF_SIZE; 
    const struct sockaddr_in addr = {0};
        
    ret = CreateSocket(AF_INET, SOCK_DGRAM, SYSTEM_PROT_CHOICE); /*UDP,
                                              socket with IPv4 domain as asked*/
    
    socket_fd = ret;
    
    RETURN_IF_BAD(ret != SERVER_FAIL, "fail creating socket\n", SERVER_FAIL);
    
    SetAddrStruct((struct sockaddr_in *)&addr);
    
    while(TRUE_VAL)
    {
        printf(ANSI_COLOR_GREEN); 
        printf("ping\n");
        printf(ANSI_COLOR_RESET); 
        
        SendMsg(socket_fd, message_buffer, BUFF_SIZE, NO_FLAGS, 
                       (const struct sockaddr *)&addr, sizeof(struct sockaddr));
        
        RETURN_IF_BAD(ret != SERVER_FAIL, "fail sending msg\n", SERVER_FAIL);
        
        ret = Recieve(socket_fd, message_buffer, BUFF_SIZE, NO_FLAGS, 
                                            (struct sockaddr *)&addr, &add_len); 
        
        RETURN_IF_BAD(ret != SERVER_FAIL, "fail recieving msg\n", SERVER_FAIL);
        
    
    }
    
    return SUCCESS;
}
