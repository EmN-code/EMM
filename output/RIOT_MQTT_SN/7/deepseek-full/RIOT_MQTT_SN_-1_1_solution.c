
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
#include "net/utils.h"
#include "net/netif.h"
#include "net/af.h"
#include "net/protnum.h"
#include "net/gnrc/ipv6/hdr.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/ipv6/ext.h"
#include "net/gnrc/ipv6/ext/frag.h"
#include "net/gnrc/ipv6/ext/rh.h"
#include "net/gnrc/ipv6/ext/dst.h"
#include "net/gnrc/ipv6/ext/mh.h"
#include "net/gnrc/ipv6/ext/opt.h"
#include "net/gnrc/ipv6/ext/hop.h"
#include "net/gnrc/ipv6/ext/route.h"
#include "net/gnrc/ipv6/ext/route/cache.h"
#include "net/gnrc/ipv6/ext/route/cache/entry.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags/flags/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags/flags/flags/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags/flags/flags/flags/flags.h"
#include "net/gnrc/ipv6/ext/route/cache/entry/state/flags/flags/flags/flags/flags/flags/flags.h"
#include "net/gncute.h"
#include "net/gnrc/netif/default.h"
#include "net/gnrc/netif/ethernet.h"
#include "net/gnrc/netif/ieee802154.h"
#include "net/gnrc/netif/ieee802154/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac/submac.h"
#include "net/gnrc/netif/ieee802154/submac/submac/submac/submac/submac/submac/submac/submac/submac/subma