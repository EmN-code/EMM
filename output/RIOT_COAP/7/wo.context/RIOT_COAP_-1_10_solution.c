#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define COAP_PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}

int main(void) {
    coap_pkt_t pdu;
    int res;

    /* Convert server IP address */
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: invalid server IP address");
        return -1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* Initialize CoAP request */
    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH);
    if (res < 0) {
        puts("Error creating CoAP request");
        return -1;
    }

    /* Set CoAP payload */
    memcpy(pdu.payload, COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);
    pdu.payload_len = sizeof(COAP_PAYLOAD) - 1;

    /* Send CoAP request */
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}