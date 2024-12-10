#include <zephyr/net/socket.h>  // for socket operations
#include <zephyr/net/coap.h>    // for CoAP operations
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request_packet;
    uint8_t payload[] = COAP_PAYLOAD;
    uint8_t coap_buf[128];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    if (coap_packet_init(&request_packet, coap_buf, sizeof(coap_buf),
        COAP_VERSION_1, COAP_TYPE_CON, 8, coap_next_token(),
        COAP_METHOD_POST, coap_next_id()) < 0) {
        printf("Failed to initialize CoAP packet\n");
        close(sock);
        return;
    }

    coap_packet_append_payload_marker(&request_packet);
    coap_packet_append_payload(&request_packet, payload, sizeof(payload) - 1);

    if (coap_packet_append_option(&request_packet, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH)) < 0) {
        printf("Failed to append URI path option\n");
        close(sock);
        return;
    }

    if (sendto(sock, request_packet.data, request_packet.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printf("CoAP request sent\n");
    close(sock);
}