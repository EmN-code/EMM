#include <zephyr/zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_RESOURCE "/coap/test"
#define COAP_DATA "Hello, COAP Cloud"

int main(void) {
    struct sockaddr_in6 addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buf[128];

    printf("Starting CoAP request\n");

    // Create UDP socket
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin6_addr);
    addr.sin6_port = htons(SERVER_PORT);

    coap_packet_init(&request, request_buf, sizeof(request_buf), 1, COAP_TYPE_CON, 
                     8, coap_next_id(), COAP_METHOD_POST, (uint8_t *) COAP_RESOURCE, strlen(COAP_RESOURCE));

    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *) COAP_DATA, strlen(COAP_DATA));

    if (sendto(sock, request.data, request.offset, 0, 
               (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Failed to send CoAP request\n");
        close(sock);
        return 1;
    }

    printf("CoAP request sent successfully\n");

    close(sock);
    return 0;
}