
#include <stdio.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

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
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload to the CoAP packet
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_pkt_write_payload(&pdu, (uint8_t *)payload, payload_len);

    // Finalize the CoAP packet
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
