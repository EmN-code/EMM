
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    if (coap_pkt_set_payload(&pdu, (uint8_t *)payload, payload_len) < 0) {
        printf("Failed to set CoAP payload\n");
        return 1;
    }

    if (coap_pkt_finalize(&pdu, 0) < 0) {
        printf("Failed to finalize CoAP packet\n");
        return 1;
    }

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
