
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
#include "net/gnrc/sixlowpan/rpl/dodag_conf.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_root.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag_no_dodag_conf.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag_no_dodag_conf_no_dodag_conf_default.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag_no_dodag_conf_no_dodag_conf_default_no_dodag_conf_default_root.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag_no_dodag_conf_no_dodag_conf_default_no_dodag_conf_default_root_no_dodag_conf_default_root_no_rpl.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_leaf_root_no_rpl_no_dhcp_no_autoconf_no_6lo_no_udp_no_tcp_no_ipv6_no_sixlowpan_no_rpl_no_dodag_no_dodag_conf_no_dodag_conf_default_no_dodag_conf_default_root_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rpl_no_dodag_conf_default_root_no_rp