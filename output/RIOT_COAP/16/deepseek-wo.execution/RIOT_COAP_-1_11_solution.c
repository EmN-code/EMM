
#include "net/gcoap.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handler logic here
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload
    const char *payload = "Hello, COAP Cloud";
    coap_pkt_set_payload(&pdu, (uint8_t *)payload, strlen(payload));

    // Convert IP address string to byte representation
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
