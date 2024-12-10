
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_handler(coap_pkt_t *pkt, struct uip_udp_conn *conn) {
    printf("CoAP response received\n");
}

int main(void) {
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, SERVER_IP);

    struct uip_udp_conn *conn = udp_new(&server_addr, UIP_HTONS(SERVER_PORT), NULL);
    if (conn == NULL) {
        printf("Failed to create UDP connection\n");
        return 1;
    }

    coap_pkt_t pkt;
    coap_init_message(&pkt, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&pkt, URI_PATH);
    coap_set_payload(&pkt, "Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(conn, &pkt, coap_handler);

    return 0;
}
