
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_lldp.h>
#include <zephyr/net/ethernet_ppp.h>
#include <zephyr/net/ethernet_vlan_filter.h>
#include <zephyr/net/ethernet_vlan_tag.h>
#include <zephyr/net/ethernet_vlan_untag.h>
#include <zephyr/net/ethernet_vlan_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_filter.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_tag.h>
#include <zephyr/net/ethernet_vlan_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag_untag.h>
#include <zephyr/ne