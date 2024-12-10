#include <stdio.h>
#include <string.h>
#include "gcoap.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     CONFIG_GCOAP_PORT
#define URI_PATH        "/coap/test"
#define COAP_BUF_SIZE   256

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[COAP_BUF_SIZE];
    const char *payload = "Hello, COAP Cloud";
    sock_udp_ep_t remote;

    /* Prepare the remote address structure */
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    /* Initialize the CoAP package */
    ssize_t len = gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    /* Set the CoAP payload */
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    /* Send the CoAP request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}

/* CoAP response handler */
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: timeout for request\n");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        printf("Error: response error\n");
    } else {
        printf("Success: received response\n");
    }
}