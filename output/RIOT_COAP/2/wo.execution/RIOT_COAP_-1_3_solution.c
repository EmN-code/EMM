#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, socket_desc_t *sock)
{
    (void)memo;
    (void)pdu;
    (void)sock;
    // Handle response if necessary
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        // Handle error
        return -1;
    }

    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) < 0) {
        // Handle error
        return -1;
    }

    if (coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD)) < 0) {
        // Handle error
        return -1;
    }

    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        // Handle error
        return -1;
    }

    return 0;
}