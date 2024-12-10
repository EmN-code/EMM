#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include "net/gcoap/req.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pkt;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;    // unused parameter

    if (pdu->code == COAP_CODE_204) {
        puts("Request successfully sent and acknowledged.");
    }
    else {
        puts("Response received (error or success) but not processed.");
    }
}

int main(void) {
    /* Initialize the local side*/
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6);

    /* Initialize the CoAP packet */
    gcoap_req_init(&pkt, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH);
    memcpy(pkt.payload, PAYLOAD, strlen(PAYLOAD));
    pkt.payload_len = strlen(PAYLOAD);

    /* Send the CoAP packet */
    if (gcoap_req_send(&buf[0], pkt.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) != 0) {
        puts("Failed to send request");
    }

    /* Spin in main thread */
    while (1) {
        xtimer_sleep(5);
    }
    return 0;
}