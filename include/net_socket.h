#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

/***********************************************************************
 *    net_socket.h: network socket functions
 ***********************************************************************
 * Copyright (C) 2013 lvyc <yongchenglv@gmail.com>
 *
 * This file is part of network protocol 
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *
 ***********************************************************************/
#include "net_common.h"
#include "net_log.h"

#ifdef __cplusplus
extern "C"{
#endif
#define	MAX_LISTEN	5

enum 
{
	NET_SOCK_NOT_CONNECTED = 0,
	NET_SOCK_CONNECTED,
	NET_SOCK_OTHER_STATUS,
};

typedef enum net_sock_block_e
{
	BLOCKING,						/* set socket fd blocking */
	NONBLOCKING,					/* set socket fd nonblock */
}net_sock_block_t;

typedef enum net_sock_type_e
{
	NET_SOCK_TYPE_TCP_CLIENT = 0,	/* tcp client socket */
	NET_SOCK_TYPE_TCP_SERVER,		/* tcp server socket */
	NET_SOCK_TYPE_UDP_CLENT,		/* udp client socket */
	NET_SOCK_TYPE_UDP_SERVER,		/* udp server socket */
}net_sock_type_t;

/* socket state struct */
typedef struct net_socket_s {
    int     fd;
    int     non_block;				/*	network socket block or no block */
    int     con_state;				/*	network socket connection status */
    int     err_num;				/*	network socket error num */
    short   sock_type;				/*	reference net_sock_type_t */
}net_sock_t;



/****************************************************
 *
 * API Function Prototypes
 *
 ****************************************************/
/**
 * @brief  Initialize the network socket
 *
 * @param  type:	net_sock_type_t type
 * @return 			The net_sock_t struct
 */
net_sock_t *net_socket_init(INPUT net_sock_type_t type);

/**
 * @breif Setting the network socket fd non block
 * @param sfd :		The net_sock_t type
 * @param type:		The net_sock_block_t type
 * @return 			T_OK or T_ERROR
 */
int net_socket_set_block_stats(INPUT net_sock_t *sfd, net_sock_block_t type);

/**
 * @brief	Close the network socket
 * @param	sfd  :	The net_sock_t type
**/
void net_socket_close(INPUT net_sock_t *sfd);

/**
 * @brief  socket connection 
 * @param  sfd  :	The net_sock_t socket struct
 * @param  host :	Connection host ip
 * @param  port :	Connection host port
 * @return			T_OK or T_ERROR
**/
int net_socket_connet(INPUT net_sock_t *sfd, INPUT const char *host, INPUT int port);

/**
 *@brief	bind localhost socket
 *@param  sfd  :	The net_sock_t socket fd
 *@param  host :	bind host ip
 *@param  port :	localhost port 
 *@return			T_OK or T_ERROR
*/
int net_socket_bind(INPUT net_sock_t *sfd, INPUT const char *host, INPUT int port);

/**
 *@brief	listen localhost socket
 *@param  sfd  :	The net_sock_t socket fd
 *@param  backlog:  the maximum length to which the queue of pending connections for sockfd may grow
 *@return			T_OK or T_ERROR
**/
int net_socket_listen(INPUT net_sock_t *sfd, INPUT int backlog);

/**
 *@brief  Accpet socket connection
 *@param  sfd  :	The net_sock_t socket fd
 *@return 			return NULL or new net_sock_t
**/
net_sock_t *net_socket_accept(INPUT net_sock_t *sfd);

/**
 *@brief Send data 
 *@param  sfd  :	The net_sock_t socket fd
 *@param  buf  :	The data
 *@param  buf_len:  The data length
 *@return			return send length
**/
int net_socket_send(INPUT net_sock_t *sfd, INPUT const char *buf, INPUT unsigned int buf_len);

/**
 *@brief  Receive data 
 *@param  sfd  :	The net_sock_t socket fd
 *@param  buf  :	The data
 *@param  buf_len:  The data length
 *@return			return receive length
**/
int net_socket_recv(INPUT net_sock_t *sfd, INPUT char *buf, INPUT int buf_len);


#ifdef __cplusplus
}
#endif

#endif
