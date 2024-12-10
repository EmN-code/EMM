#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

static void _send_coap_request(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t bytes;

    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_IP);

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    /* Send the request */
    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, 0);
    if (bytes <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }
}

int main(void) {
    /* Initialize the network interface */
    gcoap_init();

    /* Send the CoAP request */
    _send_coap_request();

    /* Sleep to allow time for response (if needed) */
    xtimer_sleep(5);

    return 0;
}