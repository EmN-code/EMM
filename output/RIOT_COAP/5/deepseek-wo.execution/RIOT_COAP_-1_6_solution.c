
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <time_units.h>
#include <stdatomic.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handling code here
}

int main(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_pkt_write_payload(&pdu, (uint8_t *)payload, payload_len);

    // Finalize the CoAP request
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
