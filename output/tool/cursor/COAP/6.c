#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void send_coap_request(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t bytes;

    // Initialize CoAP packet
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT);
    coap_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Finish CoAP packet
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send CoAP packet
    bytes = sock_udp_send(NULL, buf, len, &remote);
    if (bytes < 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void) {
    printf("RIOT CoAP client example\n");

    // Send CoAP request
    send_coap_request();

    // Sleep to allow time for response (if needed)
    xtimer_sleep(5);

    return 0;
}