#ifndef QSYSHEADERS_H
#define QSYSHEADERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>
#include <malloc.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <net/route.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if.h>

#ifdef _Q_TANK_PREVIEW_
#include "graphics.h"
#include "dmmu.h"
#include "hal.h"
#include "jz_cim.h"
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif // QSYSHEADERS_H
