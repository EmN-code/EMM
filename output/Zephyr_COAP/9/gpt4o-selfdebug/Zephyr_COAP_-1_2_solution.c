#include <stddef.h>
#include <stdint.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/net_if.h>

#define SERVER_HOST "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "coap/test"
#define COAP_REQUEST_MSG "Hello, COAP Cloud"

static struct sockaddr_in server_addr;
static struct coap_packet request;

void init_coap_request(void) {
    coap_packet_init(&request, COAP_TYPE_CON, COAP_METHOD_POST, COAP_TOKEN_LEN);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_PATH, strlen(COAP_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (u8_t *)COAP_REQUEST_MSG, strlen(COAP_REQUEST_MSG));
}

int main(void) {
    int sock;

    /* Initialize network */
    struct net_if *iface = net_if_get_default();
    if (!iface) {
        printk("No default network interface found\n");
        return -1;
    }

    net_ipaddr_copy(&server_addr.sin_addr, &iface->config.ip.ipv4->unicast[0].address.in_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    /* Create socket and connect */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to connect to server\n");
        close(sock);
        return -1;
    }

    init_coap_request();

    /* Send CoAP request */
    if (send(sock, request.data, request.offset, 0) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    close(sock);
    return 0;
}