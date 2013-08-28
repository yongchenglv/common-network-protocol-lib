#include <stdio.h>
#include "net_protocol.h"

extern const net_pro_parse_t net_pro_parse_http;
extern const net_pro_parse_t net_pro_parse_rtsp;
extern const net_pro_parse_t net_pro_parse_rtmp;
extern const net_pro_parse_t net_pro_parse_ftp;
extern const net_pro_parse_t net_pro_parse_rtp;

const net_pro_parse_t *const net_pro_parse_list[] =
{
	&net_pro_parse_http,
	&net_pro_parse_rtsp,
	&net_pro_parse_rtmp,
	&net_pro_parse_ftp,
	&net_pro_parse_rtp,
	NULL
};

static const net_pro_parse_t *get_pro_parse_type(int type)
{
	int i = 0;
	
	for (i = 0; net_pro_parse_list[i]; i++) 
	{
		if (type == net_pro_parse_list[i]->type)
			return net_pro_parse_list[i];
	}
	return NULL;
}
