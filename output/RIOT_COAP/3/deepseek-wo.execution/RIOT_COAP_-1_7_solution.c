
#include <stdlib.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

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
    remote.port = EMQX_SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    // Manually add the payload to the CoAP packet
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN) < 0) {
        puts("Error adding format option");
        return 1;
    }
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        puts("Error finishing CoAP options");
        return 1;
    }
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
