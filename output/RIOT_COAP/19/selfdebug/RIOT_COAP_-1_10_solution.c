#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t res;

    /* Initialize the remote endpoint */
    remote.port = COAP_SERVER_PORT;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: cannot convert IP address\n");
        return 1;
    }
    remote.family = AF_INET;

    /* Initialize CoAP PDU */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    /* Add payload */
    if (!coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17)) {
        printf("Error: CoAP Payload too large\n");
        return 1;
    }

    /* Send CoAP request */
    res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: could not send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: timeout receiving response\n");
        return;
    }

    printf("Response received\n");
}