
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/sixlowpan.h"
#include "net/gnrc/sixlowpan/nd.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/rpl.h"
#include "net/gnrc/sixlowpan/rpl/dodag.h"
#include "net/gnrc/sixlowpan/rpl/of.h"
#include "net/gnrc/sixlowpan/rpl/of0.h"
#include "net/gnrc/sixlowpan/rpl/mrhof.h"
#include "net/gnrc/sixlowpan/rpl/p2p.h"
#include "net/gnrc/sixlowpan/rpl/srh.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_buf.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list.h"
#include "net/gnrc/sixlowpan/rpl/srh/srh_pkt_queue_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem_list_elem.h"
#include "net/gnrc/sixlowpan/rpl/srh/sr