
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    // Add payload manually to avoid using coap_add_payload
    if (pdu.payload_len + payload_len > sizeof(buf)) {
        printf("Payload too large\n");
        return 1;
    }
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len += payload_len;

    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        printf("Failed to finish CoAP options\n");
        return 1;
    }

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
