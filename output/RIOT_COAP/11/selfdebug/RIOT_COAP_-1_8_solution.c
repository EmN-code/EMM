#include "net/sock/udp.h"
#include "net/sock/util.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;

    // Convert IP address
    int ip_conversion = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if(ip_conversion != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    // Initialize CoAP PDU with POST method
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response here
}