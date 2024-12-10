#include "string.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "assert.h"

#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH    "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *ep, uint8_t state) {
    (void)ep;
    printf("Response received: %d\n", state);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Set CoAP server details
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Convert IP string to network format
    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    assert(result == 1);

    // Initialize CoAP PDU
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    assert(len >= 0);

    // Add payload to PDU
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    // Send CoAP request
    ssize_t res = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    assert(res >= 0);

    return 0;
}