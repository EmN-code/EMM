#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <net/net_ip.h>
#include <net/net_core.h>
#include <net/net_pkt.h>
#include <net/net_if.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_BUF_SIZE 256

static struct sockaddr_in6 server_addr;
static uint8_t coap_buf[COAP_BUF_SIZE];

void main(void) {
    struct net_if *iface;
    struct coap_packet request;
    int sock;
    ssize_t len;

    // Obtain the network interface
    iface = net_if_get_default();
    if (!iface) {
        printk("No default network interface available\n");
        return;
    }

    // Prepare the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &server_addr.sin6_addr);

    // Create a CoAP socket
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Initialize CoAP request
    coap_packet_init(&request, coap_buf, sizeof(coap_buf), COAP_VERSION_1, COAP_TYPE_CON, 4, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Append URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "coap", strlen("coap"));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "test", strlen("test"));

    // Send the request
    len = sendto(sock, coap_packet_get_payload(&request), coap_packet_get_payload_len(&request), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (len < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    // Clean up
    close(sock);
}