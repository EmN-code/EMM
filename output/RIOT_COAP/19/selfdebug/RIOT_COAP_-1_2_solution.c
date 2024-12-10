#include <stdio.h>
#include <net/nanocoap.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
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

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    remote.family = AF_INET;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4);

    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
    }

    xtimer_sleep(2);

    return 0;
}