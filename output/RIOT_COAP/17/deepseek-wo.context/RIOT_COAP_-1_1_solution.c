
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

    if (pdu->hdr->code == COAP_CODE_CHANGED) {
        printf("CoAP request successful\n");
    } else {
        printf("CoAP request failed\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int res;

    // Initialize the remote endpoint with the server's IP address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_IP) == NULL) {
        printf("Error parsing server IP address\n");
        return 1;
    }

    // Initialize the CoAP packet
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    // Add the payload to the CoAP packet
    res = coap_add_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));
    if (res < 0) {
        printf("Error adding payload to CoAP request\n");
        return 1;
    }

    // Finish the CoAP packet
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (res < 0) {
        printf("Error finishing CoAP request\n");
        return 1;
    }

    // Send the CoAP request
    res = gcoap_req_send(&buf[0], res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    // Wait for the response
    xtimer_sleep(2); // Wait for 2 seconds to allow the response to be processed

    return 0;
}
