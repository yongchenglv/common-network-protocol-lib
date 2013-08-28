#include "net_protocol.h"

static int rtp_open(const char *url)
{

}

static int rtp_send_cmd(char *cmd)
{

}

static int rtp_recv_data(char *buf, int buf_len)
{

}

static void rtp_close(void)
{

}

const net_pro_parse_t net_pro_parse_rtp = {
	"RTP",
	RTP_PROTOCOL,
	rtp_open,
	rtp_send_cmd,
	rtp_recv_data,
	rtp_close
};
