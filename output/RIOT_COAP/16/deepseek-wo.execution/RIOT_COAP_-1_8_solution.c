
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stddef.h"
#include "mbox.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    // Handle response here if needed
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = HTONS(EMQX_SERVER_PORT);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(&buf[pdu.payload_offset], payload, payload_len);
    pdu.payload_len = payload_len;

    // Send CoAP request
    gcoap_req_send(buf, pdu.payload_offset + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
