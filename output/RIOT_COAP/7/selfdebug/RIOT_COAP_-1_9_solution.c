#include <string.h>
#include <arpa/inet.h>
#include <net/ipv6/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    int result;
    const char *payload = "Hello, COAP Cloud";

    // Initialize CoAP request
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, payload, strlen(payload));

    // Set remote address
    remote.port = 5683;
    result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Unable to parse IP address\n");
    }

    // Send CoAP request
    gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}