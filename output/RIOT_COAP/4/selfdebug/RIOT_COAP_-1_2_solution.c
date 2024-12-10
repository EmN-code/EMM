#include <string.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    /* Handle the response */
    printf("Response received\n");
}

int main(void) {
    coap_pkt_t pdu;
    ssize_t bytes;
    sock_udp_ep_t remote;

    printf("Starting CoAP client\n");

    /* Initialize remote endpoint details */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) == 0) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* Initialize CoAP PDU */
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) == -ENOMEM) {
        printf("Error: PDU buffer too small\n");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    /* Send request */
    bytes = gcoap_req_send(buf, pdu.buffer_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes < 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}