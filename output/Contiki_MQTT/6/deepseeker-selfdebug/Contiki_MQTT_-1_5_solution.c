
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/netif/default.h"
#include "net/netif/ethernet.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/rpl.h"
#include "net/gnrc/sixlowpan.h"
#include "net/gnrc/sixlowpan/nd.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc