#include <zephyr.h>
#include <stdio.h>
#include <net/coap.h>
#include <net/socket.h>
#include <kernel.h>

#define COAP_MAX_PACKET_SIZE 128

void main(void) {
    struct zsock_addrinfo hints, *res;
    int sock;
    struct sockaddr_in *addr_in;
    static const char send_buf[] = "Hello, COAP Cloud";
    struct coap_packet cpkt;
    uint8_t pkt_buf[COAP_MAX_PACKET_SIZE];
    const char *msg_type = COAP_TYPE_CON; // Correct usage instead of undefined COAP_TOKEN_NONE

    printf("Starting CoAP client\n");

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int ret = zsock_getaddrinfo("47.102.103.1", "5683", &hints, &res);
    if (ret != 0) {
        printf("Failed to resolve server: %d\n", ret);
        return;
    }

    addr_in = (struct sockaddr_in *)res->ai_addr;
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create UDP socket: %d\n", sock);
        return;
    }

    ret = coap_packet_init(&cpkt, pkt_buf, sizeof(pkt_buf), COAP_VERSION_1,
        msg_type, COAP_MAKE_TOKEN(NULL, 0), 
        res->ai_addr, NULL);
    if (ret < 0) {
        printf("Failed to initialize CoAP packet: %d\n", ret);
        return;
    }
    
    coap_packet_append_option(&cpkt, COAP_OPTION_URI_PATH, (uint8_t *)"coap/test", 9);
    coap_packet_append_payload_marker(&cpkt);
    coap_packet_append_payload(&cpkt, (uint8_t *)send_buf, sizeof(send_buf) - 1);

    ret = zsock_sendto(sock, cpkt.data, cpkt.offset, 0, res->ai_addr, sizeof(*addr_in));
    if (ret < 0) {
        printf("Failed to send CoAP request: %d\n", ret);
    } else {
        printf("CoAP message sent successfully\n");
    }

    zsock_freeaddrinfo(res);
    zsock_close(sock);
}