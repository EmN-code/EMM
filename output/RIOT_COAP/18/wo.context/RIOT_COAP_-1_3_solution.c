#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu) {
        printf("Response received: Code: %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error: failed to send CoAP request");
    }

    return 0;
}
