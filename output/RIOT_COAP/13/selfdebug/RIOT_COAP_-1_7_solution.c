#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"  // required for inet_pton

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

#ifndef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#endif

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from the server
    printf("Response received: %s\n", pdu->payload);
}

int main(void) {
    // Setting up the remote server address
    remote.family = AF_INET;
    remote.port = EMQX_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == -1) {
        printf("Failed to initialize CoAP PDU\n");
        return -1;
    }

    // Copy payload to PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res == -1) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}