#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[128];
    ssize_t res;
    coap_pkt_t pdu;

    // Resolve the server address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    // Initialize the CoAP packet
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        puts("Error: unable to initialize CoAP request");
        return 1;
    }

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Finish the CoAP packet
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        puts("Error: unable to finish CoAP packet");
        return 1;
    }

    // Send the CoAP request
    res = sock_udp_send(NULL, buf, pdu.payload_len + pdu.hdr_len, &remote);
    if (res < 0) {
        puts("Error: unable to send CoAP request");
        return 1;
    }

    puts("CoAP request sent successfully");

    return 0;
}