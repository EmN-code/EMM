
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_ppp.h>
#include <zephyr/net/ethernet_tap.h>
#include <zephyr/net/ethernet_virt.h>
#include <zephyr/net/ethernet_wifi.h>
#include <zephyr/net/ethernet_l2.h>
#include <zephyr/net/ethernet_stats.h>
#include <zephyr/net/ethernet_context.h>
#include <zephyr/net/ethernet_api.h>
#include <zephyr/net/ethernet_eth.h>
#include <zephyr/net/ethernet_ip.h>
#include <zephyr/net/ethernet_tcp.h>
#include <zephyr/net/ethernet_udp.h>
#include <zephyr/net/ethernet_icmp.h>
#include <zephyr/net/ethernet_igmp.h>
#include <zephyr/net/ethernet_arp.h>
#include <zephyr/net/ethernet_nd.h>
#include <zephyr/net/ethernet_dhcp.h>
#include <zephyr/net/ethernet_dns.h>
#include <zephyr/net/ethernet_http.h>
#include <zephyr/net/ethernet_mqtt.h>
#include <zephyr/net/ethernet_coap.h>
#include <zephyr/net/ethernet_lldp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void send_coap_request(int sock, const struct sockaddr *addr) {
    struct coap_packet request;
    struct coap_option options[1];
    uint8_t payload[] = "Hello, COAP Cloud";
    int ret;

    ret = coap_packet_init(&request, options, 1, COAP_TYPE_CON, COAP_METHOD_POST, 0, NULL, 0, payload, sizeof(payload) - 1);
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        return;
    }

    ret = coap_append_option_int(&request, COAP_OPTION_URI_PATH, strlen(URI_PATH), URI_PATH);
    if (ret < 0) {
        printk("Failed to append URI path option\n");
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        return;
    }

    ret = coap_packet_append_payload(&request, payload, sizeof(payload) - 1);
    if (ret < 0) {
        printk("Failed to append payload\n");
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, addr, sizeof(*addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }
}

int main(void) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    send_coap_request(sock, (struct sockaddr *)&addr);

    close(sock);
    return 0;
}
