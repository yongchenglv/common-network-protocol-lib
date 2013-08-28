AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = main
main_SOURCES = $(top_srcdir)/src/main.c  \
			$(top_srcdir)/src/net_socket.c	\
			$(top_srcdir)/src/net_protocol.c \
			$(top_srcdir)/src/net_http.c 	\
			$(top_srcdir)/src/net_rtsp.c 	\
			$(top_srcdir)/src/net_rtmp.c 	\
			$(top_srcdir)/src/net_rtp.c 	\
			$(top_srcdir)/src/net_ftp.c 

INCLUDES = -I./include
#include_HEADERS = $(top_srcdir)/include/net_socket.h $(top_srcdir)/include/net_log.h $(top_srcdir)/include/net_common.h
