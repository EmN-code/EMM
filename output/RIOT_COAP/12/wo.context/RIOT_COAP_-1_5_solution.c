#include <string.h>
#include "net/gcoap.h"
#include "net/ip.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %u\n", coap_get_code(pdu));
    }
    else {
        printf("Response timeout\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    int res;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", res);
    }

    xtimer_sleep(5);

    return 0;
}
