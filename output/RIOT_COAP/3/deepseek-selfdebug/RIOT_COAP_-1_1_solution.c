
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_pdu_parse(pdu->payload, pdu->payload_len, resp_buf, sizeof(resp_buf));
    if (len > 0) {
        printf("Received CoAP response: %s\n", resp_buf);
    }
    else {
        puts("Error parsing CoAP response");
    }
}

int main(void)
{
    // Initialize the gcoap module
    gcoap_init();

    // Prepare the CoAP request
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Set the remote IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Initialize the CoAP packet
    int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    // Add payload to the CoAP packet
    res = coap_add_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));
    if (res < 0) {
        puts("Error adding payload to CoAP request");
        return 1;
    }

    // Finish the CoAP packet
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (res < 0) {
        puts("Error finishing CoAP request");
        return 1;
    }

    // Send the CoAP request
    res = gcoap_req_send(buf, res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
