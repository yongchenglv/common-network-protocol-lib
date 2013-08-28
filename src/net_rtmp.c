#include "net_protocol.h"

static int rtmp_open(const char *url)
{

}

static int rtmp_send_cmd(char *cmd)
{

}

static int rtmp_recv_data(char *buf, int buf_len)
{

}

static void rtmp_close(void)
{

}

const net_pro_parse_t net_pro_parse_rtmp = {
	"RTMP",
	RTMP_PROTOCOL,
	rtmp_open,
	rtmp_send_cmd,
	rtmp_recv_data,
	rtmp_close
};
