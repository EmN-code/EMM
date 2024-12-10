#include <zephyr/zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_REQUEST_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 addr;
    int sock;
    struct coap_packet request;
    uint8_t buf[256];
    
    // Create a socket
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Error creating socket\n");
        return;
    }

    // Configure server address
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET6, COAP_SERVER_IP, &addr.sin6_addr);

    // Initialize CoAP packet
    coap_packet_init(&request, buf, sizeof(buf), 1, COAP_TYPE_CON, 1234, 1, COAP_METHOD_POST, NULL, 0);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_REQUEST_PAYLOAD, strlen(COAP_REQUEST_PAYLOAD));

    // Send CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Error sending CoAP request\n");
        close(sock);
        return;
    }

    printf("CoAP POST request sent\n");
    close(sock);
}