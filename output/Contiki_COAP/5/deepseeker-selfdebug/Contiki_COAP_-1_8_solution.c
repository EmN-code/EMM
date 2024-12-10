
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_handler(coap_pkt_t *pdu, struct sockaddr *src) {
    printf("CoAP response received\n");
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    uint8_t buf[COAP_MAX_PDU_SIZE];
    coap_pkt_init(&pdu, buf, COAP_MAX_PDU_SIZE, COAP_TYPE_CON, COAP_POST, 0);
    coap_pkt_set_uri(&pdu, URI_PATH, strlen(URI_PATH));
    coap_pkt_set_payload(&pdu, "Hello, COAP Cloud", 16);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);
    sock_udp_send(&sock, pdu.hdr, coap_pkt_get_total_size(&pdu), &remote);
    sock_udp_recv(&sock, coap_handler, COAP_MAX_PDU_SIZE, COAP_RESPONSE_TIMEOUT, NULL);

    return 0;
}
