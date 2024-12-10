#include <stdio.h>
#include <string.h>
#include <coap.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/gnrc/netif.h>
#include <net/gnrc.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"

#define BUFFER_SIZE 128

typedef struct {
    coap_pkt_t pkt;
    uint8_t buf[BUFFER_SIZE];
} coap_packet_t;

void send_coap_request(void) {
    sock_udp_ep_t server_ep = { .family = AF_INET6 };
    server_ep.port = atoi(SERVER_PORT);

    if (ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server_ep, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    coap_packet_t coap_pkt;

    coap_pkt_init(&coap_pkt.pkt, coap_pkt.buf, BUFFER_SIZE, COAP_TYPE_CON);
    coap_pkt_hdr_set_code(&coap_pkt.pkt, COAP_METHOD_POST);
    coap_pkt_uri_path_add(&coap_pkt.pkt, URI_PATH);

    size_t len = coap_pkt_payload_put(&coap_pkt.pkt, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    if (len == 0) {
        puts("Error: unable to set payload");
        sock_udp_close(&sock);
        return;
    }

    if (sock_udp_send(&sock, coap_pkt.buf, coap_pkt.pkt.hdr.len, &server_ep) < 0) {
        puts("Error: unable to send CoAP request");
    }
    else {
        puts("CoAP request sent successfully");
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("Starting CoAP client");
    send_coap_request();
    return 0;
}