
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_tap.h>
#include <zephyr/net/ethernet_l2.h>
#include <zephyr/net/ethernet_stats.h>
#include <zephyr/net/ethernet_context.h>
#include <zephyr/net/ethernet_api.h>
#include <zephyr/net/ethernet_eth.h>
#include <zephyr/net/ethernet_ip.h>
#include <zephyr/net/ethernet_tcp.h>
#include <zephyr/net/ethernet_udp.h>
#include <zephyr/net/ethernet_icmp.h>
#include <zephyr/net/ethernet_arp.h>
#include <zephyr/net/ethernet_ipv6.h>
#include <zephyr/net/ethernet_ipv4.h>
#include <zephyr/net/ethernet_ppp.h>
#include <zephyr/net/ethernet_ppp_mgmt.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr/net/ethernet_ppp_api.h>
#include <zephyr/net/ethernet_ppp_eth.h>
#include <zephyr/net/ethernet_ppp_ip.h>
#include <zephyr/net/ethernet_ppp_tcp.h>
#include <zephyr/net/ethernet_ppp_udp.h>
#include <zephyr/net/ethernet_ppp_icmp.h>
#include <zephyr/net/ethernet_ppp_arp.h>
#include <zephyr/net/ethernet_ppp_ipv6.h>
#include <zephyr/net/ethernet_ppp_ipv4.h>
#include <zephyr/net/ethernet_ppp_l2.h>
#include <zephyr/net/ethernet_ppp_stats.h>
#include <zephyr/net/ethernet_ppp_context.h>
#include <zephyr