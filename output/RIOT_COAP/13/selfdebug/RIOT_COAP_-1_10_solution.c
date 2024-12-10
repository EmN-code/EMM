#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/inet/addr.h>
#include <xtimer.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pkt, const sock_udp_ep_t *remote) {
    (void) remote;
    if (memo->state == GCOAP_MEMO_RX) {
        /* Handle the successful response here */
        printf("Received response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    /* Set up remote endpoint */
    remote.port = COAP_PORT;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    /* Initialize the CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_hdr_set_payload(&pdu, strlen(PAYLOAD));

    /* Send the CoAP request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send request");
        return 1;
    }

    puts("CoAP request sent");

    /* Wait for response */
    xtimer_sleep(10);
    return 0;
}