
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_tap.h>
#include <zephyr/net/ethernet_ppp.h>
#include <zephyr/net/ethernet_lldp.h>
#include <zephyr/net/ethernet_dsa.h>
#include <zephyr/net/ethernet_fdd.h>
#include <zephyr/net/ethernet_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_mgmt.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_vlan.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_bridge.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_tap.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_ppp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_lldp.h>
#include <zephyr/net/ethernet_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_fdd_dsa.h>
#include <zephyr