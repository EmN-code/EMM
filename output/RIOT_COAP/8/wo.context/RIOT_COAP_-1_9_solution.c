#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

#ifndef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#endif

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    /* Handle the response */
    printf("Response received from %s\n", COAP_SERVER_IP);
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    /* Initialize CoAP request */
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    /* Send CoAP request */
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}