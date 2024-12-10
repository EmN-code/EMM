#include <xtimer.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/coap.h>
#include <net/gnrc.h>
#include <net/af.h>
#include <kernel.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define SERVER_PATH "/coap/test"

#define REQUEST_WAIT_INTERVAL (5U * US_PER_SEC)

void send_coap_request(void) {
    gnrc_pktsnip_t *pkt;
    gnrc_pktsnip_t *payload;
    ipv6_addr_t addr;
    size_t len;
    char *message = "Hello, COAP Cloud";

    if (ipv6_addr_from_str(&addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return;
    }

    len = strlen(message);
    payload = gnrc_pktbuf_add(NULL, message, len, GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: unable to copy message to packet buffer");
        return;
    }

    pkt = gnrc_coap_req_build(NULL, GNRC_COAP_REQ_POST, SERVER_PATH);
    if (pkt == NULL) {
        puts("Error: unable to build CoAP packet");
        gnrc_pktbuf_release(payload);
        return;
    }

    LL_APPEND(pkt, payload);
    
    if (gnrc_coap_req_send(pkt, GNRC_NETTYPE_UDP, &addr, GEMALLOC_AF_UNSPEC, atoi(SERVER_PORT), 0) < 0) {
        puts("Error: unable to send CoAP request");
        gnrc_pktbuf_release(pkt);
        return;
    }

    puts("CoAP request sent successfully!");
}

int main(void) {
    while (1) {
        send_coap_request();
        xtimer_sleep(REQUEST_WAIT_INTERVAL);
    }
    return 0;
}