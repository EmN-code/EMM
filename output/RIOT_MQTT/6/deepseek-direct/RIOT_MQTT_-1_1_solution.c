
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/ipv6/addr.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netreg.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/udp.h>
#include <net/gnrc/tcp.h>
#include <net/gnrc/sixlowpan.h>
#include <net/gnrc/sixlowpan/nd.h>
#include <net/gnrc/sixlowpan/frag.h>
#include <net/gnrc/sixlowpan/iphc.h>
#include <net/gnrc/sixlowpan/rpl.h>
#include <net/gnrc/sixlowpan/rpl/dodag.h>
#include <net/gnrc/sixlowpan/rpl/dodag_conf.h>
#include <net/gnrc/sixlowpan/rpl/dodag_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_metric.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_info.h>
#include <net/gnrc/sixlowpan/rpl/dodag_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry_route_table_entry