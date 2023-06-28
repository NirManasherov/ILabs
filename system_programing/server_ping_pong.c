#include <string.h>		/*	strcpy		*/

#include "dynamic_vector.h" 
#include "utils.h"
#include "tcp_udp_ex1.c"

#define MAX(x, y) ((x<y) ? y : x)

int ServerDriver(void); 

int main(void)
{
    return ServerDriver();     
}

int ServerDriver(void)
{
    d_vector_ty *vector = NULL;
    int logger_fd = 0; 
    int tcp_socket_accept = 1; 
    int  udp_socket = 1; 
    int status = 1; 
    int is_continue = TRUE_VAL;
    int max_fd = 0; 
    
    vector = VectorCreate(SIZE_BUFF, sizeof(int));
    RETURN_IF_BAD_CLEAN((NULL != vector), "VectorCreate FAIL", 1, 
                                                         VectorDestroy(vector));
    
    logger_fd = open(./server_logger, O_WRONLY | O_CREAT | O_APPEND); 
    RETURN_IF_BAD_CLEAN((-1 != fd_logger), "open FAIL", 1, 
                                                         VectorDestroy(vector));
    
    CreateSocket(AF_INET, SOCK_NONBLOCK | SOCK_STREAM, SYSTEM_PROT_CHOICE);
    RETURN_IF_BAD_CLEAN((-1 != tcp_socket), "CreateSocket FAIL", 1, 
                                    {VectorDestroy(vector); close(fd_logger);});
                                    
    
    udp_socket = CreateSocket(AF_INET, SOCK_NONBLOCK | SOCK_DGRAM, 0);
    RETURN_IF_BAD_CLEAN((-1 != udp_socket), "CreateSocket FAIL", 1, 
                 {VectorDestroy(vector); close(fd_logger); close(tcp_socket);});

    /*for nonblock for STDIN*/
    status = fcntl(STDIN_FILENO, F_SETFL, fcntl(STDOUT_FILENO, F_GETFL) | 
                                                                    O_NONBLOCK);
    
    
    
    return SUCCESS;    
}
