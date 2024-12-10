#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handler function to process response
    printf("Response received\n");
}

int main() {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));

    // Initializing remote server address
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    ssize_t result = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}