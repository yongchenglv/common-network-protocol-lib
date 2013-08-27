#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <fcntl.h>

#include "net_socket.h"

// just for TCP_DELAY
#ifndef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#ifdef HAVE_SOCKADDR_LEN
#define SET_SOCKADDR_SIN_LEN(var) var.sin_len = sizeof var
#else
#define SET_SOCKADDR_SIN_LEN(var)
#endif
/*adr,prt must be in network order*/
#define MAKE_SOCKADDR_IN(var, adr, prt) \
	struct sockaddr_in var; \
	var.sin_family = AF_INET; \
	var.sin_addr.s_addr = (adr); \
	var.sin_port = (prt); \
	SET_SOCKADDR_SIN_LEN(var);

net_sock_t *net_socket_init(INPUT net_sock_type_t type)
{
	int	fd = -1;
    short sock_type  = 0;
    net_sock_t	*sfd = NULL;

	switch(type)
	{
		case NET_SOCK_TYPE_TCP_CLIENT:
		case NET_SOCK_TYPE_TCP_SERVER:
			if ((fd = socket(AF_INET, SOCK_STREAM, 0)) > 0)
			{
				sock_type = SOCK_STREAM;
			}
			break;
		case NET_SOCK_TYPE_UDP_CLENT:
		case NET_SOCK_TYPE_UDP_SERVER:
			if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
			{
				sock_type = SOCK_DGRAM;
			}
			break;
		default:
			fd = -1;
			break;
	}

	if (fd < 0)
	{
		NET_MSG(NET_ERROR,"file: %s line: %d  net_socket_init socket %s \n", 
					__FILE__, __LINE__, strerror(errno));
		return NULL;
	}
	// malloc & free 还没有封装 
	if ((sfd = (net_sock_t *)malloc(sizeof(net_sock_t))) == NULL)
	{
		close(fd);
		NET_MSG(NET_ERROR,"file: %s line: %d net_socket_init malloc %s \n", 
					__FILE__, __LINE__, strerror(errno));
		return NULL;
	}

	// set socket default values
	sfd->fd = fd;
	sfd->con_state = NET_SOCK_CONNECTED;
	sfd->non_block = FALSE;
	sfd->err_num = 0;
	sfd->sock_type = sock_type;

	return (sfd);
}

int net_socket_set_block_stats(INPUT net_sock_t *sfd, net_sock_block_t type)
{
	int flag = -1;
	int ret = -1;	

	if ((NULL == sfd) || (sfd->fd < 0))
	{
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_set_noblocking errror!", 
						__FILE__, __LINE__);
		return T_ERROR;
	}

	flag = fcntl(sfd->fd, F_GETFL, 0);
	if (flag < 0)
	{
		goto error;
	}
    
	if (BLOCKING == type)
	{
		if (fcntl(sfd->fd, F_SETFL, flag & (~O_NONBLOCK)) < 0)
			goto error;
	}
	else if (NONBLOCKING == type) 
	{
		if (fcntl(sfd->fd, F_SETFL, flag | O_NONBLOCK) < 0)
			goto error;
	}
	else{
		// do nothing
	}
	return T_OK;

error:
	NET_MSG(NET_WARNING,"file: %s line: %d net_socket_set_block_stats type=%d error %s !", 
						__FILE__, __LINE__, type, strerror(errno));
	return T_ERROR;
}

void net_socket_close(INPUT net_sock_t *sfd)
{
	if (NULL != sfd) {
		close(sfd->fd);
		free(sfd);
	}
}

int net_socket_connet(INPUT net_sock_t *sfd, INPUT const char *host, INPUT int port)
{
    struct hostent *server;	

	if ((sfd == NULL) || (host == NULL) || (port < 0))
	{
		NET_MSG(NET_ERROR,"file: %s line: %d  net_socket_connet args error!\n",
					__FILE__, __LINE__);
		goto con_err;
	}

	server = gethostbyname(host);

	if (server == NULL)
	{
		NET_MSG(NET_ERROR,"file: %d line: %d  net_socket_connet gethostbyname error %s !\n", 
					__FILE__, __LINE__, strerror(errno));
		//sfd->errNum = error;
		goto con_err;
    }

	MAKE_SOCKADDR_IN(remotename, inet_addr(server->h_name), htons(port));

	if (connect(sfd->fd, (struct sockaddr*)&remotename, sizeof(remotename)) != 0)
	{
		NET_MSG(NET_WARNING,"connect server faillure %s!", strerror(errno));
		//sfd->errNum = error;
		goto con_err;
	}	
	sfd->con_state = NET_SOCK_CONNECTED;
	return T_OK;
con_err:
	sfd->con_state = NET_SOCK_NOT_CONNECTED;
	return T_ERROR;
}

int net_socket_bind(INPUT net_sock_t *sfd, INPUT const char *host, INPUT int port)
{
	int bind_flag = 1;
	int ret = T_ERROR;
    struct hostent *server;	

	if ((sfd == NULL) || (host == NULL) || (port < 0))
	{
		NET_MSG(NET_ERROR,"file: %s line: %d net_socket_bind args error!",
					__FILE__, __LINE__);
		goto bind_err;
	}

	server = gethostbyname(host);
	if (server == NULL)
	{
		//sfd->errNum = error;
		NET_MSG(NET_ERROR,"file: %s line: %d net_socket_bind gethostbyname error %s !\n", 
					__FILE__, __LINE__, strerror(errno));
		goto bind_err;
    }

	MAKE_SOCKADDR_IN(localname, inet_addr(server->h_name), htons(port));

	if (setsockopt(sfd->fd, SOL_SOCKET, SO_REUSEADDR, &bind_flag, sizeof(bind_flag)) == -1)
	{
		perror("setscokopt error!");
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_bind setsockopt error %s !\n", 
					__FILE__, __LINE__, strerror(errno));
		//sfd->errNum = error;
		goto bind_err;
	}

	ret = bind(sfd->fd, (struct sockaddr*)&localname, sizeof(localname));

	if(ret == 0)
	{
			return T_OK;
	} else {
		//sfd->errNum = error;
		NET_MSG(NET_ERROR,"file: %s line: %d  net_socket_bind bind error %s !\n", 
					__FILE__, __LINE__, strerror(errno));
	}
bind_err:
	return T_ERROR;
}

int net_socket_listen(INPUT net_sock_t *sfd, INPUT int backlog)
{
	int ret = T_ERROR;

	if (sfd == NULL) {
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_listen  sfd ==NULL !\n",
					__FILE__, __LINE__);
		return T_ERROR;
	}

	if (backlog < 0) {
		backlog = MAX_LISTEN;
	}

	ret = listen(sfd->fd, backlog);
	
	return (ret == 0) ? (T_OK) : (T_ERROR);
}

net_sock_t *net_socket_accept(INPUT net_sock_t *sfd)
{
	struct sockaddr_in 	cli_addr;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	int tcp_nodelay = 1;
	int c_fd = -1;
	int retval;
	net_sock_t *new_sfd = NULL;
	struct linger so_linger;

	if (sfd == NULL)
	{
		NET_MSG(NET_WARNING,"file: %s line: %d  net_socket_accept  sfd ==NULL !\n",
					__FILE__, __LINE__);
		return NULL;
	}

	c_fd = accept(sfd->fd, (struct sockaddr *)&cli_addr, &cli_len);

	if (c_fd == -1) 
	{
		perror("accpet error!");		
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_accept  accpet: %s\n", 
					__FILE__, __LINE__, strerror(errno));
		return NULL;
	}	

	if ((new_sfd =(net_sock_t *)malloc(sizeof(net_sock_t))) == NULL)
	{
		close(c_fd);
		NET_MSG(NET_WARNING,"file: %s line: %d  net_socket_accept  malloc %s\n",
					__FILE__, __LINE__, strerror(errno));
		return NULL;
	}

	new_sfd->fd = c_fd;
	new_sfd->con_state = NET_SOCK_CONNECTED;
	new_sfd->non_block = sfd->non_block;
	new_sfd->err_num = 0;
	// set tcp nodelay
	retval = setsockopt(new_sfd->fd, IPPROTO_TCP, TCP_NODELAY, &tcp_nodelay, sizeof(tcp_nodelay));
	if (retval != 0)
	{
		perror("set tcp nodelay error!");
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_accept  setsockopt TCP_NODELAY %s\n", 
							__FILE__, __LINE__, strerror(errno));
	}
	// enable socket flush out buffered data before closing	 socket
	//This option applies only to connection-oriented protocols such as TCP
	so_linger.l_onoff = 1;  // Turn linger option on.
  	so_linger.l_linger = 3; // Linger time in seconds
	retval = setsockopt(new_sfd->fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
	if (retval != 0)
	{
		perror("setsockopt SO_LINGER ERROR");
		NET_MSG(NET_WARNING,"file: %s line: %d net_socket_accept  setsockopt SO_LINGER %s\n", 
					__FILE__, __LINE__, strerror(errno));
	}

	return (new_sfd);
}

int net_socket_send(INPUT net_sock_t *sfd, INPUT const char *buf, INPUT unsigned int buf_len)
{
	ssize_t s_size = -1;

	if ((sfd == NULL) || (sfd->con_state != NET_SOCK_CONNECTED))
	{
		NET_MSG(NET_WARNING, "file: %s line: %d net_socket_send failure!\n", __FILE__, __LINE__);
		return T_ERROR;
	}
	
	s_size = send(sfd->fd, buf, buf_len, 0);

	return s_size;
}

int net_socket_recv(INPUT net_sock_t *sfd, INPUT char *buf, INPUT int buf_len)
{
	size_t  last = 0;
    ssize_t recv_size = -1;

	if ((sfd == NULL) || (sfd->con_state != NET_SOCK_CONNECTED)) {
		return T_ERROR;
	}

    while (1) {
        recv_size = recv(sfd->fd, buf + last, buf_len, 0);

        if (recv_size == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            } else {
                return T_ERROR;
            }
        }

        if (recv_size == 0) {
         	goto recv_done;
        }

        last += recv_size;

        if ((buf_len -= recv_size) == 0) {
            break;
        }
    }

recv_done:
	return last;
}

