#include "net/nanocoap.h"
#include "gcoap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from server\n");
    printf("Response code: %d\n", coap_get_code(pdu));
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    size_t len;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Initialize and configure the CoAP packet */
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    gcoap_opt_finish(&pdu, 17);

    /* Add the data to be sent in the payload */
    len += 17;  // Adjust packet length to include the payload

    /* Send the packet using the generic CoAP send function */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }

    return 0;
}