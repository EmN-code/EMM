#include <stdio.h>
#include <stdlib.h>
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static const char *post_data = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Remote unused
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        printf("Response to POST was truncated\n");
    }
    else {
        printf("POST request complete\n");
    }
}

int main(void) {
    printf("RIOT CoAP example: send POST request\n");

    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address string to network address
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address\n");
        return 1;
    }

    // Initialize the CoAP packet
    int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Error initializing request: %d\n", res);
        return 1;
    }

    // Set the payload for the POST request
    memcpy(pdu.payload, post_data, strlen(post_data));
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request\n");
    }

    return 0;
}