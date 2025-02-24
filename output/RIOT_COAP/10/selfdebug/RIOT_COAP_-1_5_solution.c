#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from the server
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .port = COAP_SERVER_PORT };
    coap_pkt_t pdu;
    ssize_t result;
    
    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Malformed IP address\n");
        return -1;
    }
    else if (result < 0) {
        printf("inet_pton not supported\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    return 0;
}