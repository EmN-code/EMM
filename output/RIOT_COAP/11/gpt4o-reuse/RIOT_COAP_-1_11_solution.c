
#include "gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gnrc/ipv4.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/ipv6/nc.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/ipv6/whitelist.h"
#include "net/gnrc/ipv6/blacklist.h"
#include "net/gnrc/ipv6/hdr.h"
#include "net/gnrc/ipv6/ext.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/srh.h"
#include "net/gnrc/ipv6/ext/esp.h"
#include "net/gnrc/ipv6/ext/ah.h"
#include "net/gnrc/ipv6/ext/mob.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/calipso.h"
#include "net/gnrc/ipv6/ext/mpl.h"
#include "net/gnrc/ipv6/ext/shim6.h"
#include "net/gnrc/ipv6/ext/ilnp.h"
#include "net/gnrc/ipv6/ext/4over6.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6over4.h"
#include "net/gnrc/ipv6/ext/6ove