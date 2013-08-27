#ifndef __NET_LOG_H__
#define __NET_LOG_H__

/***********************************************************************
 *    net_log.h: network protocol log print
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef __GNUC__
#define CHECK_FORMAT_ARGS(m,n) __attribute__((format(printf,m,n)))
#else
#define CHECK_FORMAT_ARGS(m,n)
#endif

#define	MAX_LINE (1024 * 8)
typedef enum  net_log_level_e{
    NET_DEBUG = 0, 
	NET_MESSAGE,
    NET_WARNING,
    NET_ERROR,
    NET_EXIT
} net_log_level_t;

#define NET_LOGV(lev, line) 						\
do {												\
	fputs(line, stderr);							\
	fflush(stderr);									\
	if(lev == NET_ERROR) abort();					\
	if(lev == NET_EXIT) exit(EXIT_SUCCESS);			\
} while(0)

static inline void net_log(net_log_level_t lev, const char *fmt, va_list vl)
{
    char line[MAX_LINE];
	line[0] = 0;
   	vsnprintf(line + strlen(line), sizeof(line) - strlen(line), fmt, vl);
	NET_LOGV(lev, line);
}

static inline void CHECK_FORMAT_ARGS(2,3) NET_MSG(net_log_level_t lev, const char *fmt,...)
{
	va_list va;
	va_start(va, fmt);
	net_log(lev, fmt, va);
	va_end(va);
}

#define ERROR_IF(expr) 								\
do {                            					\
    if(expr){                                   	\
        fprintf(stderr, "%s\n", strerror(errno));   \
        exit(EXIT_FAILURE);                         \
    }                                               \
} while(0)

#endif
