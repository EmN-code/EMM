
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pkt.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/sixlowpan.h"
#include "net/gnrc/sixlowpan/nd.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/reass.h"
#include "net/gnrc/sixlowpan/ctx.h"
#include "net/gnrc/sixlowpan/iphc