#include <zephyr/net/websocket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_buf.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/device_mmio.h>

#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define REQUEST_PAYLOAD "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

void send_coap_post_request(void) {
    struct sockaddr_in6 server_addr;
    struct coap_packet request;
    uint8_t request_data[128];
    int sock;
    size_t len;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_ADDR, &server_addr.sin6_addr);
    server_addr.sin6_port = htons(SERVER_PORT);

    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, REQUEST_PAYLOAD, strlen(REQUEST_PAYLOAD));

    len = coap_packet_get_total_size(&request);
    if (sendto(sock, request.data, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    close(sock);
}

int main(void) {
    printf("Starting CoAP client example\n");
    send_coap_post_request();
    return 0;
}