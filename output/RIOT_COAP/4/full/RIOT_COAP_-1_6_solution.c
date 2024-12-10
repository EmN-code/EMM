#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <assert.h>

// CoAP server address
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

// Payload
#define PAYLOAD "Hello, COAP Cloud"

static gcoap_resp_handler_t _resp_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote) {
    // Handle the response here
    (void)pdu;  // Suppressing unused variable warning
    (void)remote;
    if (req_state == GCOAP_MEMO_COMPLETE) {
        puts("Response received");
    }
    else {
        puts("Response not received");
    }
    return 0;
}

int main(void) {
    // Initialize server endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Prepare CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}