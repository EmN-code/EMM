#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"
#define PAYLOAD        "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}