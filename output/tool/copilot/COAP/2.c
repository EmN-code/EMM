#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void send_coap_request(void) {
    uint8_t buf[128];
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    ssize_t bytes = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote);
    if (bytes <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void) {
    printf("RIOT CoAP client example\n");

    while (1) {
        send_coap_request();
        xtimer_sleep(10); // Send request every 10 seconds
    }

    return 0;
}