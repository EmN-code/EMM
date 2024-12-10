#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define SERVER_PATH "/coap/test"
#define COAP_METHOD_POST 2

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; /* unused */
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("GCOAP_MEMO_TIMEOUT\n");
    } else {
        printf("Response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, SERVER_PATH);
    int err = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (err < 0) {
        printf("Error finishing CoAP options\n");
        return 1;
    }

    coap_pkt_t *pkt = &pdu;
    const char *payload = "Hello, COAP Cloud";
    coap_opt_add_format(pkt, (unsigned int)COAP_FORMAT_TEXT_PLAIN);
    coap_payload(pkt, (const uint8_t *)payload, strlen(payload));

    gcoap_req_send(buf, pkt->payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP request sent\n");
    return 0;
}
