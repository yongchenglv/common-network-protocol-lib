#include "net_protocol.h"

static int http_open(const char *url)
{

}

static int http_send_cmd(char *cmd)
{

}

static int http_recv_data(char *buf, int buf_len)
{

}

static void http_close(void)
{

}

const net_pro_parse_t net_pro_parse_http = {
	"HTTP",
	HTTP_PROTOCOL,
	http_open,
	http_send_cmd,
	http_recv_data,
	http_close
};
