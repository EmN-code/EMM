
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <net/mqtt.h>
#include <net/net_mgmt.h>
#include <net/net_event.h>
#include <net/net_conn_mgr.h>
#include <net/wifi_mgmt.h>
#include <net/ethernet.h>
#include <net/ethernet_mgmt.h>
#include <net/ethernet_vlan.h>
#include <net/ethernet_bridge.h>
#include <net/ethernet_tap.h>
#include <net/ethernet_ppp.h>
#include <net/ethernet_lldp.h>
#include <net/ethernet_mld.h>
#include <net/ethernet_igmp.h>
#include <net/ethernet_arp.h>
#include <net/ethernet_ipv4.h>
#include <net/ethernet_ipv6.h>
#include <net/ethernet_icmp.h>
#include <net/ethernet_icmpv6.h>
#include <net/ethernet_tcp.h>
#include <net/ethernet_udp.h>
#include <net/ethernet_sockets.h>
#include <net/ethernet_sockets_offload.h>
#include <net/ethernet_sockets_internal.h>
#include <net/ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_mgmt.h>
#include <net/ethernet_sockets_ethernet_vlan.h>
#include <net/ethernet_sockets_ethernet_bridge.h>
#include <net/ethernet_sockets_ethernet_tap.h>
#include <net/ethernet_sockets_ethernet_ppp.h>
#include <net/ethernet_sockets_ethernet_lldp.h>
#include <net/ethernet_sockets_ethernet_mld.h>
#include <net/ethernet_sockets_ethernet_igmp.h>
#include <net/ethernet_sockets_ethernet_arp.h>
#include <net/ethernet_sockets_ethernet_ipv4.h>
#include <net/ethernet_sockets_ethernet_ipv6.h>
#include <net/ethernet_sockets_ethernet_icmp.h>
#include <net/ethernet_sockets_ethernet_icmpv6.h>
#include <net/ethernet_sockets_ethernet_tcp.h>
#include <net/ethernet_sockets_ethernet_udp.h>
#include <net/ethernet_sockets_ethernet_sockets.h>
#include <net/ethernet_sockets_ethernet_sockets_offload.h>
#include <net/ethernet_sockets_ethernet_sockets_internal.h>
#include <net/ethernet_sockets_ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_vlan.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_bridge.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_tap.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_ppp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_lldp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_mld.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_igmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_arp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_ipv4.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_ipv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_icmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_icmpv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_tcp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_udp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_offload.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_internal.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_vlan.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_bridge.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tap.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ppp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_lldp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mld.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_igmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_arp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv4.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmpv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tcp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_udp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_offload.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_internal.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_vlan.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_bridge.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tap.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ppp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_lldp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mld.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_igmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_arp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv4.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmpv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tcp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_udp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_offload.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_internal.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_vlan.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_bridge.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tap.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ppp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_lldp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mld.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_igmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_arp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv4.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_ipv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_icmpv6.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_tcp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_udp.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_offload.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_internal.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_event.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_net_conn_mgr.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_wifi_mgmt.h>
#include <net/ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_sockets_ethernet_mgmt.h>
#include <net/etherne