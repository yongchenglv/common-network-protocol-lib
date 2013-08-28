#include "net_protocol.h"

static int rtsp_open(const char *url)
{

}

static int rtsp_send_cmd(char *cmd)
{

}

static int rtsp_recv_data(char *buf, int buf_len)
{

}

static void rtsp_close(void)
{

}

const net_pro_parse_t net_pro_parse_rtsp = {
	"RTSP",
	RTSP_PROTOCOL,
	rtsp_open,
	rtsp_send_cmd,
	rtsp_recv_data,
	rtsp_close
};
