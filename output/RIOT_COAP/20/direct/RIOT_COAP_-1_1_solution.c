#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/gnrc/netif.h>
#include <net/gnrc.h>
#include <net/coap.h>
#include <net/sock.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"

#define COAP_MSG_TYPE COAP_TYPE_CON
#define COAP_CODE COAP_METHOD_POST

#define COAP_CONTENT "Hello, COAP Cloud"

static void send_coap_request(void) {
    coap_pkt_t pdu;
    uint8_t buf[128];

    size_t len = coap_build_hdr(buf, COAP_TYPE_CON, NULL, 0);
    if (len == 0) {
        printf("Error building CoAP header\n");
        return;
    }

    len += coap_build_request(buf + len, COAP_METHOD_POST, URI_PATH);
    if (len == 0) {
        printf("Error building CoAP request\n");
        return;
    }

    len += coap_set_payload(buf, len, COAP_CONTENT, strlen(COAP_CONTENT));

    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);
    remote.port = atoi(SERVER_PORT);

    if (sock_udp_send(NULL, buf, len, &remote) < 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }
}

int main(void) {
    while (1) {
        send_coap_request();
        xtimer_sleep(5); // Wait for 5 seconds before sending the next request
    }
    return 0;
}