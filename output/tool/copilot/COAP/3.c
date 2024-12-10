#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _send_coap_request(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, NULL);
    if (bytes_sent <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    puts("RIOT CoAP client example");

    // Wait for network to be ready
    xtimer_sleep(5);

    // Send CoAP request
    _send_coap_request();

    return 0;
}