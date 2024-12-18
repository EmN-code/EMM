
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_l2.h>
#include <zephyr/net/ethernet_stats.h>
#include <zephyr/net/ethernet_eth.h>
#include <zephyr/net/ethernet_ip.h>
#include <zephyr/net/ethernet_tcp.h>
#include <zephyr/net/ethernet_udp.h>
#include <zephyr/net/ethernet_icmp.h>
#include <zephyr/net/ethernet_arp.h>
#include <zephyr/net/ethernet_dhcp.h>
#include <zephyr/net/ethernet_dns.h>
#include <zephyr/net/ethernet_http.h>
#include <zephyr/net/ethernet_mqtt.h>
#include <zephyr/net/ethernet_coap.h>
#include <zephyr/net/ethernet_lwm2m.h>
#include <zephyr/net/ethernet_ota.h>
#include <zephyr/net/ethernet_ota_http.h>
#include <zephyr/net/ethernet_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_coap.h>
#include <zephyr/net/ethernet_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_http.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_mqtt.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_coap.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_lwm2m.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota_dfu_ota.h>
#include <zephyr/net/ethernet_ota_dfu_ota_dfu_ota_dfu_ota_dfu