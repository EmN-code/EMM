
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/sock/udp.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/ipv6/hdr.h"
#include "net/gnrc/ipv6/ext.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/shim6.h"
#include "net/gnrc/ipv6/ext/mpl.h"
#include "net/gnrc/ipv6/ext/rpl.h"
#include "net/gnrc/ipv6/ext/mld.h"
#include "net/gnrc/ipv6/ext/mldv2.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry_list_entry.h"
#include "net/gnrc/ipv6/ext/mldv2_mcast_group_list_entry_list_entry_list_entry_list_entry_list_entry_list_entr