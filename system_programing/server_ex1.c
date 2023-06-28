/*UDP SERVER*/
#include <string.h>		/*	strcpy		*/

#include "utils.h"
#include "tcp_udp_ex1.c"

int ServerDriver(void); 

int main(void)
{
    return ServerDriver();     
}

int ServerDriver(void)
{
    int ret = 1;
    int socket_fd = 0; 
    char message_buffer[BUFF_SIZE] = {0};
    socklen_t add_len = BUFF_SIZE; 
    struct sockaddr_in addr = {0};
    
    ret = CreateSocket(AF_INET, SOCK_DGRAM, SYSTEM_PROT_CHOICE); /*UDP, socket      
                                                    with IPv4 domain as asked*/

    RETURN_IF_BAD(ret != SERVER_FAIL, "fail creating socket\n", SERVER_FAIL);
    
    socket_fd = ret; 
    
    SetAddrStruct((struct sockaddr_in *)&addr);
    
    ret = BindSocket(socket_fd , &addr, sizeof(addr)); 
    
    RETURN_IF_BAD(ret == SERVER_SUCCESS, "fail binding socket\n", SERVER_FAIL);
    
    while(TRUE_VAL)
    {
        ret = Recieve(socket_fd, message_buffer, BUFF_SIZE, NO_FLAGS, 
                                    (struct sockaddr *)&addr, &add_len); 
        
        RETURN_IF_BAD(ret != SERVER_FAIL, "fail recieving msg\n", SERVER_FAIL);
        
        printf(ANSI_COLOR_MAGENTA); 
        printf("pong\n");
        printf(ANSI_COLOR_RESET); 
        
        memset(message_buffer, 0, BUFF_SIZE); 
        
        strcpy(message_buffer, "server message"); 
        
        SendMsg(socket_fd, message_buffer, BUFF_SIZE, NO_FLAGS, 
                       (const struct sockaddr *)&addr, sizeof(struct sockaddr));
    	
    	RETURN_IF_BAD(ret != SERVER_FAIL, "fail sending msg\n", SERVER_FAIL);
    }
    
    return SERVER_SUCCESS;
}

