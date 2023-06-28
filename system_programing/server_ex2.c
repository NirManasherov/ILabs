/*TCP SERVER*/
#include <string.h>		/*	strcpy		*/

#include "utils.h"
#include "tcp_udp_ex2.c"

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
    int new_fd = 1; 
    
     ret = CreateSocket(AF_INET, SOCK_STREAM, SYSTEM_PROT_CHOICE); /*TCP,
                                              socket with IPv4 domain as asked*/
    
    socket_fd = ret;   
    
    RETURN_IF_BAD(ret != SERVER_FAIL, "fail creating socket\n", SERVER_FAIL);
    
    SetAddrStruct((struct sockaddr_in *)&addr);
    
    ret = BindSocket(socket_fd , &addr, sizeof(addr)); 
    
    RETURN_IF_BAD(ret == SERVER_SUCCESS, "fail binding socket\n", SERVER_FAIL);
    
    ListenToSocket(socket_fd, MAX_CONNECTIONS);
    
    new_fd = AcceptClient(socket_fd, (struct sockaddr *)&addr, &add_len);
    
    while(TRUE_VAL)
    {
        ret = Recieve(new_fd, message_buffer, BUFF_SIZE, NO_FLAGS);
        
        RETURN_IF_BAD(ret != SERVER_FAIL, "fail recieving msg\n", FAIL);
        
        printf(ANSI_COLOR_MAGENTA); 
        printf("pong\n");
        printf(ANSI_COLOR_RESET); 
        
        memset(message_buffer, 0, BUFF_SIZE); 
        
        strcpy(message_buffer, "server message"); 
        
        SendMsg(new_fd, message_buffer, BUFF_SIZE, NO_FLAGS);
    	
    	RETURN_IF_BAD(ret != SERVER_FAIL, "fail sending msg\n", SERVER_FAIL);
    }
    
    return SERVER_SUCCESS; 
}
