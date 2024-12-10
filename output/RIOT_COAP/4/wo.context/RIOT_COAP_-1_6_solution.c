// Description and Solution:
// The provided code will implement sending a CoAP POST request using RIOT OS
// on an ESP32. It configures a CoAP client to send the message "Hello, COAP Cloud"
// to the CoAP server at the specified IP and port, with the defined URI path.
// This code resolves the compilation issues found in earlier attempts by declaring
// required macros, correct type usages, and includes response handling.

#include "net/sock/udp.h"
#include "net/gcoap.h"

#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

event_queue_t queue;

typedef struct {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
} state_t;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)memo; (void)remote;
    if (!pdu) {
        puts("No response received");
    } else {
        printf("Response: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    state_t state;
    coap_pkt_t pdu;
    int len;
    size_t payload_len = strlen(PAYLOAD);

    // Resolve the remote address
    memset(&state.remote, 0, sizeof(state.remote));
    state.remote.family = AF_INET6;
    state.remote.port = CONFIG_GCOAP_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)state.remote.addr.ipv6, EMQX_SERVER_IP) == NULL) {
        puts("Error: unable to parse remote address");
        return 1;
    }

    // Initialize the CoAP PDU
    len = gcoap_req_init(&pdu, state.buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        puts("Error: unable to initialize PDU");
        return 1;
    }

    // Insert payload into PDU
    memcpy(pdu.payload, PAYLOAD, payload_len);
    len += payload_len;

    // Send the CoAP request
    if (gcoap_req_send(state.buf, len, &state.remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error: unable to send CoAP request");
        return 1;
    }

    // Block to handle responses
    event_queue_run(&queue);

    return 0;
}
