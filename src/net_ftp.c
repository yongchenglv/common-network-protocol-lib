#include "net_protocol.h"

static int ftp_open(const char *url)
{

}

static int ftp_send_cmd(char *cmd)
{

}

static int ftp_recv_data(char *buf, int buf_len)
{

}

static void ftp_close(void)
{

}

const net_pro_parse_t net_pro_parse_ftp = {
	"FTP",
	FTP_PROTOCOL,
	ftp_open,
	ftp_send_cmd,
	ftp_recv_data,
	ftp_close
};
