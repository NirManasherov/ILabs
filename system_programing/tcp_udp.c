#include <sys/types.h>          /*  socket macros, reciever struct            */
#include <sys/socket.h>         /*  socket, bind , recvfrom                   */
#include <netinet/in.h>         /*  bind                                      */
#include <assert.h>             /*  assert                                    */
#include <arpa/inet.h>          /*  htons                                     */

enum {SERVER_SUCCESS = 0, 
      SYSTEM_PROT_CHOICE = 0,
      FALSE_VAL = 0, 
      NO_FLAGS = 0, 
      SERVER_FAIL = -1, 
      TRUE_VAL = 1, 
      BUFF_SIZE = 100
      }; 

/*returns the fd of the new socket on success and -1 on fail*/
int CreateSocket(int domain_, int type_, int protocol)
{
    int socket_fd = -1; 
    
    /*support different machines connected by the internet*/
    socket_fd = socket(domain_, type_, protocol); 
    return socket_fd; 
}

void SetAddrStruct(struct sockaddr_in *addr_, char *ip_)
{
    assert(NULL != addr_);
    
    addr_->sin_family = AF_INET;
    addr_->sin_port = htons(1025); /*for big/little endien correction*/
    addr_->sin_addr.s_addr = inet_addr(ip); /*IP for local host*/
}

/*return 0 on success and -1 on fail*/
int BindSocket(int socket_fd_, const struct sockaddr_in *addr_, socklen_t 
                                                                       addrlen_)
{
    int bind_status = 1; 
   
    assert(0 != addrlen_);
    assert(NULL != addr_); 
    
    bind_status = bind(socket_fd_, (const struct sockaddr *)addr_, addrlen_);
    
    return bind_status;
}

/*return num of bytes read or -1 in fail*/
int RecieveUDP(int sockfd_, void *buf_, size_t len_, int flags_,
                        struct sockaddr *src_addr_, socklen_t *addrlen_)
{
    int num_bytes_recieved = -1; 
    
    assert(NULL != buf_);
    assert(NULL != src_addr_);
    assert(NULL != addrlen_); 
    assert(0 < *addrlen_);
    
    num_bytes_recieved = recvfrom(sockfd_, buf_, len_, flags_, src_addr_, 
                                                                     addrlen_);
    
    RETURN_IF_BAD(num_bytes_recieved != SERVER_FAIL, "fail recieving data",
                                                                   SERVER_FAIL);
                                                                     
    return num_bytes_recieved; 
}

/*returns the number of bytes sent return -1 on error*/
ssize_t SendMsgUDP(int sockfd_, const void *buf_, size_t len_, int flags_,
                      const struct sockaddr *dest_addr_, socklen_t addrlen_)
{
	ssize_t num_bytes_sent = 0; 
	
	assert(NULL != buf_);
	assert(0 < len_); 
	assert(NULL != dest_addr_); 
	assert(0 < addrlen_); 
	
	num_bytes_sent = sendto(sockfd_, buf_, len_, flags_,dest_addr_, addrlen_); 
	
	RETURN_IF_BAD(num_bytes_sent != SERVER_FAIL, "fail recieving data",
                                                                   SERVER_FAIL);
                                                                     
	
	return num_bytes_sent;
}

/*return num of bytes read or -1 in fail*/
int RecieveTCP(int sockfd_, void *buf_, size_t len_, int flags_)
{
    int num_bytes_recieved = -1; 
    
    assert(NULL != buf_);
    assert(0 < len_);
    
    num_bytes_recieved = recv(sockfd_, buf_, len_, flags_);
    
    RETURN_IF_BAD(num_bytes_recieved != SERVER_FAIL, "fail recieving data",
                                                                   SERVER_FAIL);
                                                                     
    return num_bytes_recieved; 
}

/*returns the number of bytes sent return -1 on error*/
ssize_t SendMsgTCP(int sockfd_, const void *buf_, size_t len_, int flags_)
{
	ssize_t num_bytes_sent = 0; 
	
	assert(NULL != buf_);
	assert(0 < len_); 
		
	num_bytes_sent = send(sockfd_, buf_, len_, flags_); 
	
	RETURN_IF_BAD(num_bytes_sent != SERVER_FAIL, "fail recieving data",
                                                                   SERVER_FAIL);
                                                                     
	
	return num_bytes_sent;
}

int ListenToSocket(int sockfd_, int backlog_)
{
    int status = -1; 

    assert(0 < backlog_); 
    
    status = listen(sockfd_, backlog_); 
    
    RETURN_IF_BAD(status != SERVER_FAIL, "fail listening to socket",
                                                                   SERVER_FAIL);
    
    return SUCCESS;
}

int ConnectClient(int sockfd_, const struct sockaddr *addr_, socklen_t addrlen_)
{
    int fd = 1; 
    
    assert(NULL != addr_);
    assert(0 < addrlen_); 
    
    fd = connect(sockfd_, addr_, addrlen_);
    
    RETURN_IF_BAD(fd != SERVER_FAIL, "fail connecting client", SERVER_FAIL);
    
    return SUCCESS;
}

int AcceptClient(int sockfd_, struct sockaddr *addr_, socklen_t *addlen_)
{
    int fd = -1; 
    
    assert(NULL != addr_);
    assert(NULL != addlen_);
    
    fd = accept(sockfd_, addr_, addlen_);  
    
    RETURN_IF_BAD(fd != SERVER_FAIL, "fail accepting client", SERVER_FAIL);
    
    return fd; 
}
