#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <net/nanocoap.h>
#include <net/nanocoap/cache.h>

#define COAP_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    // Check the response code
    int code = coap_get_code(pdu);
    printf("Response code: %i\n", code);
}

int main(void) {
    // Initialize buffer for the PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    
    // Define the remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr);
    
    // Initialize the CoAP PDU
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NONCON); // Set to NON-confirmable

    // Add payload
    const char* msg = "Hello, COAP Cloud";
    memcpy(pdu.payload, msg, strlen(msg));
    len += strlen(msg);

    // Send the request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}