#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define SERVER_IPV6_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define GCOAP_PDU_BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static uint8_t buf[GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pdu;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC)
        puts("Response too large, truncated");
    else if (memo->state != GCOAP_MEMO_UNUSED) {
        printf("Response %u.%u
", coap_get_code_class(pdu), coap_get_code_detail(pdu));
    }
    else {
        puts("Failed to receive response");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_IPV6_ADDR, &remote.addr.ipv6);

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    uint8_t *payload = coap_payload(&pdu);
    memcpy(payload, PAYLOAD, strlen(PAYLOAD));
    coap_set_payload_len(&pdu, strlen(PAYLOAD));

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Wait some time for the response
    xtimer_sleep(5);

    return 0;
}