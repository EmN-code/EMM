
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/netopt.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netreg.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/udp.h>
#include <net/gnrc/tcp.h>
#include <net/gnrc/pktdump.h>
#include <net/gnrc/netif/hdr.h>
#include <net/gnrc/netif/default.h>
#include <net/gnrc/netif/ethernet.h>
#include <net/gnrc/netif/ieee802154.h>
#include <net/gnrc/netif/ieee802154_submac.h>
#include <net/gnrc/netif/ieee802154_submac_params.h>
#include <net/gnrc/netif/ieee802154_submac_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_types.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_params.h>
#include <net/gnrc/netif/ieee802154_submac_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal_api_internal.h>
#include <net/gnr