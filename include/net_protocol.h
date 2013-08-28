#ifndef __NET_PROTOCOL_H__
#define __NET_PROTOCOL_H__
/***********************************************************************
 *    net_protocal.h: network protocol common functions defitions
 ***********************************************************************
 * Copyright (C) 2013 lvyc <yongchenglv@gmail.com>
 *
 * This file is part of network protocol 
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

#define	HTTP_PORT	80
#define FTP_PORT	21
#define RTSP_PORT	554

enum
{
	HTTP_PROTOCOL = 0,
	RTSP_PROTOCOL,			/* standard real time stream protocol, not microsoft RTSP */
	RTMP_PROTOCOL,
	FTP_PROTOCOL,
	RTP_PROTOCOL
};

typedef struct net_pro_parse_s
{
	/*	network protocol name */
	const char *net_pro_name;	
	/*	network protocol type, the same with net_pro_name */
	int	type;
	/*	open URL */
	int (*open)(const char *url);
	/*	send command to remote server */
	int (*send_cmd)(char *cmd);	
	/*	receive data from remote server */
	int (*recv_data)(char *buf, int buf_len);
	/*	close  all */
	void (*close)(void);
}net_pro_parse_t;


#endif
