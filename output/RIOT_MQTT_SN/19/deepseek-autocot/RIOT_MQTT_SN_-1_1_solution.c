
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/netif.h"
#include "net/netopt.h"
#include "net/udp.h"
#include "net/ipv6.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/ipv6/hdr.h"
#include "net/gnrc/ipv6/ext.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/dstopts.h"
#include "net/gnrc/ipv